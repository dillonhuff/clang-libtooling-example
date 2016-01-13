#include <iostream>

#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Driver/Options.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

static cl::OptionCategory MyToolCategory("My tool options");
static int numForLoops = 0;
StatementMatcher LoopMatcher = forStmt().bind("forLoop"); //forStmt(hasLoopInit(declStmt(hasSingleDecl(varDecl(hasInitializer(integerLiteral(equals(0)))))))).bind("forLoop");

class LoopPrinter : public MatchFinder::MatchCallback {
public:
  virtual void run(const MatchFinder::MatchResult &Result) {
    if (const ForStmt *FS = Result.Nodes.getNodeAs<clang::ForStmt>("forLoop")) {
      clang::LangOptions LangOpts;
      LangOpts.CPlusPlus = true;
      clang::PrintingPolicy Policy(LangOpts);
      std::string TypeS;
      llvm::raw_string_ostream s(TypeS);
      FS->printPretty(errs(), 0, Policy);
      numForLoops++;
      //FS->dump();
    }
  }
};

int main(int argc, const char **argv) {
  std::string errMsg = "err";
  std::string dir = "/Users/dillon/CWorkspace/git/";
  auto cdb = CompilationDatabase::loadFromDirectory(dir, errMsg);
  auto files = cdb->getAllFiles();
  // for (auto file : files) {
  //   cout << file << endl;
  // }
  ClangTool Tool(*(cdb.get()), cdb->getAllFiles());

  LoopPrinter Printer;
  MatchFinder Finder;
  Finder.addMatcher(LoopMatcher, &Printer);

  int result = Tool.run(newFrontendActionFactory(&Finder).get());
  cout << "Number of for loops: " << numForLoops << endl;
  return result;
}
