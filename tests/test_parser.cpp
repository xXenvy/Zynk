#include <gtest/gtest.h>

#include "../src/parsing/include/parser.hpp"
#include "../src/parsing/include/lexer.hpp"
#include "../src/common/include/errors.hpp"

TEST(ParserTest, parseVariableDeclaration) {
    Lexer lexer("var a: int = 1;");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::VariableDeclaration);

    const auto var = static_cast<ASTVariableDeclaration*>(program->body.front().get());

    ASSERT_EQ(var->varType, ASTValueType::Integer);
    ASSERT_EQ(var->name, "a");
    const auto value = static_cast<ASTValue*>(var->value.get());
    ASSERT_NE(value, nullptr);
    ASSERT_EQ(value->value, "1");
}

TEST(ParserTest, parseMultipleVariableDeclarations) {
    Lexer lexer("var a: int = 1;\nvar b: float = 1.0;\n"
        "var c: bool = true; \nvar d: string = \"Test\";");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 4);

    for (const auto& var : program->body) {
        ASSERT_EQ(var->type, ASTType::VariableDeclaration);
    }
}

TEST(ParserTest, parseFunctionDeclaration) {
    Lexer lexer("def main() -> null {\n    println(10);\n}");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::FunctionDeclaration);

    const auto function = static_cast<ASTFunction*>(program->body.front().get());
    ASSERT_EQ(function->name, "main");
    ASSERT_EQ(function->returnType, ASTValueType::None);
    ASSERT_EQ(function->body.size(), 1);
    ASSERT_EQ(function->body.front()->type, ASTType::Print);

    const auto print = static_cast<ASTPrint*>(function->body.front().get());
    ASSERT_NE(print, nullptr);
    ASSERT_TRUE(print->newLine);
    const auto printValue = static_cast<ASTValue*>(print->expression.get());
    ASSERT_NE(printValue, nullptr);
    ASSERT_EQ(printValue->value, "10");
}

TEST(ParserTest, parseFunctionDeclarationWithReturnType) {
    Lexer lexer("def add() -> int {\n    return a + b;\n}");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::FunctionDeclaration);

    const auto function = static_cast<ASTFunction*>(program->body.front().get());
    ASSERT_EQ(function->name, "add");
    ASSERT_EQ(function->returnType, ASTValueType::Integer);
    ASSERT_EQ(function->body.size(), 1);
    ASSERT_EQ(function->body.front()->type, ASTType::Return);

    const auto returnStmt = static_cast<ASTReturn*>(function->body.front().get());
    ASSERT_NE(returnStmt, nullptr);

    const auto returnValue = static_cast<ASTBinaryOperation*>(returnStmt->value.get());
    ASSERT_NE(returnValue, nullptr);
    ASSERT_EQ(returnValue->op, "+");

    const auto left = static_cast<ASTVariable*>(returnValue->left.get());
    const auto right = static_cast<ASTVariable*>(returnValue->right.get());
    ASSERT_NE(left, nullptr);
    ASSERT_NE(right, nullptr);
    ASSERT_EQ(left->name, "a");
    ASSERT_EQ(right->name, "b");
}

TEST(ParserTest, parseEmptyFunction) {
    Lexer lexer("def main() -> null {\n}");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::FunctionDeclaration);

    const auto function = static_cast<ASTFunction*>(program->body.front().get());
    ASSERT_EQ(function->name, "main");
    ASSERT_EQ(function->body.size(), 0);
}

TEST(ParserTest, parseVariableDeclarationWithBinaryOperation) {
    Lexer lexer("var a: float = b + 1.0;");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::VariableDeclaration);

    const auto var = static_cast<ASTVariableDeclaration*>(program->body.front().get());
    ASSERT_EQ(var->name, "a");
    ASSERT_EQ(var->varType, ASTValueType::Float);

    const auto operation = static_cast<ASTBinaryOperation*>(var->value.get());
    ASSERT_NE(operation, nullptr);
    ASSERT_EQ(operation->op, "+");

    const auto left = static_cast<ASTVariable*>(operation->left.get());
    const auto right = static_cast<ASTValue*>(operation->right.get());
    ASSERT_NE(left, nullptr);
    ASSERT_NE(right, nullptr);
    ASSERT_EQ(left->name, "b");
    ASSERT_EQ(right->value, "1.0");
}

TEST(ParserTest, parseVariableDeclarationWithComplexExpression) {
    Lexer lexer("var a: float = 1 + 5 * b;");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::VariableDeclaration);

    const auto var = static_cast<ASTVariableDeclaration*>(program->body.front().get());
    ASSERT_EQ(var->name, "a");
    ASSERT_EQ(var->varType, ASTValueType::Float);

    const auto operation = static_cast<ASTBinaryOperation*>(var->value.get());
    ASSERT_NE(operation, nullptr);
    ASSERT_EQ(operation->op, "+");

    const auto leftValue = static_cast<ASTValue*>(operation->left.get());
    const auto rightOperation = static_cast<ASTBinaryOperation*>(operation->right.get());

    ASSERT_NE(leftValue, nullptr);
    ASSERT_NE(rightOperation, nullptr);
    ASSERT_EQ(leftValue->value, "1");
    ASSERT_EQ(rightOperation->op, "*");

    const auto rightLeftValue = static_cast<ASTValue*>(rightOperation->left.get());
    const auto rightRightVariable = static_cast<ASTVariable*>(rightOperation->right.get());

    ASSERT_NE(rightLeftValue, nullptr);
    ASSERT_NE(rightRightVariable, nullptr);
    ASSERT_EQ(rightLeftValue->value, "5");
    ASSERT_EQ(rightRightVariable->name, "b");
}

TEST(ParserTest, parsePrintAndPrintlnCalls) {
    Lexer lexer("print(0);\nprintln(true);");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 2);

    int newLine = 0;
    for (const auto& printBase : program->body) {
        const auto print = static_cast<ASTPrint*>(printBase.get());
        ASSERT_NE(print, nullptr);
        ASSERT_EQ(print->newLine, newLine);
        newLine++;
    }
}

TEST(ParserTest, parseVariableModify) {
    Lexer lexer("a = 42;");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::VariableModify);

    const auto varModify = static_cast<ASTVariableModify*>(program->body.front().get());
    ASSERT_EQ(varModify->name, "a");

    const auto newValue = static_cast<ASTValue*>(varModify->value.get());
    ASSERT_NE(newValue, nullptr);
    ASSERT_EQ(newValue->value, "42");
}

TEST(ParserTest, parseVariableModifyWithExpression) {
    Lexer lexer("x = 5 + y * 3;");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::VariableModify);

    const auto varModify = static_cast<ASTVariableModify*>(program->body.front().get());
    ASSERT_EQ(varModify->name, "x");

    const auto operation = static_cast<ASTBinaryOperation*>(varModify->value.get());
    ASSERT_NE(operation, nullptr);
    ASSERT_EQ(operation->op, "+");

    const auto leftValue = static_cast<ASTValue*>(operation->left.get());
    const auto rightOperation = static_cast<ASTBinaryOperation*>(operation->right.get());

    ASSERT_NE(leftValue, nullptr);
    ASSERT_NE(rightOperation, nullptr);
    ASSERT_EQ(leftValue->value, "5");
    ASSERT_EQ(rightOperation->op, "*");

    const auto rightLeftVariable = static_cast<ASTVariable*>(rightOperation->left.get());
    const auto rightRightValue = static_cast<ASTValue*>(rightOperation->right.get());

    ASSERT_NE(rightLeftVariable, nullptr);
    ASSERT_NE(rightRightValue, nullptr);
    ASSERT_EQ(rightLeftVariable->name, "y");
    ASSERT_EQ(rightRightValue->value, "3");
}

TEST(ParserTest, parseSimpleIfStatement) {
    Lexer lexer("if (a > b) println(10);");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::Condition);

    const auto condition = static_cast<ASTCondition*>(program->body.front().get());
    const auto conditionExpression = static_cast<ASTBinaryOperation*>(condition->expression.get());

    ASSERT_NE(condition, nullptr);
    ASSERT_EQ(condition->body.size(), 1);
    ASSERT_EQ(conditionExpression->op, ">");

    const auto printStatement = static_cast<ASTPrint*>(condition->body.front().get());
    const auto printValue = static_cast<ASTValue*>(printStatement->expression.get());

    ASSERT_NE(printStatement, nullptr);
    ASSERT_EQ(printValue->value, "10");
}

TEST(ParserTest, parseIfElseStatement) {
    Lexer lexer("if (x == 5) println(\"x is 5\"); else println(\"x is not 5\");");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::Condition);

    const auto condition = static_cast<ASTCondition*>(program->body.front().get());
    const auto conditionExpression = static_cast<ASTBinaryOperation*>(condition->expression.get());

    ASSERT_NE(condition, nullptr);
    ASSERT_EQ(conditionExpression->op, "==");

    ASSERT_EQ(condition->body.size(), 1);
    const auto ifPrint = static_cast<ASTPrint*>(condition->body.front().get());
    const auto ifPrintValue = static_cast<ASTValue*>(ifPrint->expression.get());

    ASSERT_EQ(ifPrintValue->value, "x is 5");

    ASSERT_EQ(condition->elseBody.size(), 1);
    const auto elsePrint = static_cast<ASTPrint*>(condition->elseBody.front().get());
    const auto elsePrintValue = static_cast<ASTValue*>(elsePrint->expression.get());
    ASSERT_EQ(elsePrintValue->value, "x is not 5");
}

TEST(ParserTest, parseSimpleReadInput) {
    Lexer lexer("readInput();");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::ReadInput);

    const auto read = static_cast<ASTReadInput*>(program->body.front().get());
    ASSERT_NE(read, nullptr);
    ASSERT_EQ(read->out, nullptr);
}

TEST(ParserTest, parseReadInputWithText) {
    Lexer lexer("readInput(\"Enter your name: \");");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::ReadInput);

    const auto read = static_cast<ASTReadInput*>(program->body.front().get());
    ASSERT_NE(read, nullptr);

    const auto text = static_cast<ASTValue*>(read->out.get());
    ASSERT_NE(text, nullptr);
    ASSERT_EQ(text->value, "Enter your name: ");
}

TEST(ParserTest, parseTypeCastFromStringToInt) {
    Lexer lexer("var a: int = int(\"123\");");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::VariableDeclaration);

    const auto var = static_cast<ASTVariableDeclaration*>(program->body.front().get());
    ASSERT_EQ(var->name, "a");
    ASSERT_EQ(var->varType, ASTValueType::Integer);

    const auto cast = static_cast<ASTTypeCast*>(var->value.get());
    ASSERT_NE(cast, nullptr);
    ASSERT_EQ(cast->castType, ASTValueType::Integer);

    const auto castValue = static_cast<ASTValue*>(cast->value.get());
    ASSERT_NE(castValue, nullptr);
    ASSERT_EQ(castValue->value, "123");
}

TEST(ParserTest, parseTypeCastFromIntToString) {
    Lexer lexer("var a: string = string(123);");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::VariableDeclaration);

    const auto var = static_cast<ASTVariableDeclaration*>(program->body.front().get());
    ASSERT_EQ(var->name, "a");
    ASSERT_EQ(var->varType, ASTValueType::String);

    const auto cast = static_cast<ASTTypeCast*>(var->value.get());
    ASSERT_NE(cast, nullptr);
    ASSERT_EQ(cast->castType, ASTValueType::String);

    const auto castValue = static_cast<ASTValue*>(cast->value.get());
    ASSERT_NE(castValue, nullptr);
    ASSERT_EQ(castValue->value, "123");
}

TEST(ParserTest, parseTypeCastFromStringToFloat) {
    Lexer lexer("var a: float = float(\"123.45\");");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::VariableDeclaration);

    const auto var = static_cast<ASTVariableDeclaration*>(program->body.front().get());
    ASSERT_EQ(var->name, "a");
    ASSERT_EQ(var->varType, ASTValueType::Float);

    const auto cast = static_cast<ASTTypeCast*>(var->value.get());
    ASSERT_NE(cast, nullptr);
    ASSERT_EQ(cast->castType, ASTValueType::Float);

    const auto castValue = static_cast<ASTValue*>(cast->value.get());
    ASSERT_NE(castValue, nullptr);
    ASSERT_EQ(castValue->value, "123.45");
}

TEST(ParserTest, parseTypeCastFromBoolToString) {
    Lexer lexer("var a: string = string(true);");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::VariableDeclaration);

    const auto var = static_cast<ASTVariableDeclaration*>(program->body.front().get());
    ASSERT_EQ(var->name, "a");
    ASSERT_EQ(var->varType, ASTValueType::String);

    const auto cast = static_cast<ASTTypeCast*>(var->value.get());
    ASSERT_NE(cast, nullptr);
    ASSERT_EQ(cast->castType, ASTValueType::String);

    const auto castValue = static_cast<ASTValue*>(cast->value.get());
    ASSERT_NE(castValue, nullptr);
    ASSERT_EQ(castValue->value, "true");
}

TEST(ParserTest, parseNegativeNumber) {
    Lexer lexer("var a: int = -5;");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::VariableDeclaration);

    const auto var = static_cast<ASTVariableDeclaration*>(program->body.front().get());
    ASSERT_EQ(var->name, "a");
    ASSERT_EQ(var->varType, ASTValueType::Integer);

    const auto value = static_cast<ASTValue*>(var->value.get());
    ASSERT_NE(value, nullptr);
    ASSERT_EQ(value->value, "-5");
    ASSERT_EQ(value->valueType, ASTValueType::Integer);
}

TEST(ParserTest, parseBinaryOperationWithNegativeNumber) {
    Lexer lexer("var a: int = -1 + -5;");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::VariableDeclaration);

    const auto var = static_cast<ASTVariableDeclaration*>(program->body.front().get());
    ASSERT_EQ(var->name, "a");
    ASSERT_EQ(var->varType, ASTValueType::Integer);

    const auto binOp = static_cast<ASTBinaryOperation*>(var->value.get());
    ASSERT_NE(binOp, nullptr);
    ASSERT_EQ(binOp->op, "+");

    const auto leftValue = static_cast<ASTValue*>(binOp->left.get());
    ASSERT_NE(leftValue, nullptr);
    ASSERT_EQ(leftValue->value, "-1");
    ASSERT_EQ(leftValue->valueType, ASTValueType::Integer);

    const auto rightValue = static_cast<ASTValue*>(binOp->right.get());
    ASSERT_NE(rightValue, nullptr);
    ASSERT_EQ(rightValue->value, "-5");
    ASSERT_EQ(rightValue->valueType, ASTValueType::Integer);
}

TEST(ParserTest, parseAndOperation) {
    Lexer lexer("var result: bool = true && false;");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::VariableDeclaration);

    const auto var = static_cast<ASTVariableDeclaration*>(program->body.front().get());
    ASSERT_EQ(var->name, "result");
    ASSERT_EQ(var->varType, ASTValueType::Bool);

    const auto operation = static_cast<ASTAndOperation*>(var->value.get());
    ASSERT_NE(operation, nullptr);

    const auto leftValue = static_cast<ASTValue*>(operation->left.get());
    const auto rightValue = static_cast<ASTValue*>(operation->right.get());

    ASSERT_NE(leftValue, nullptr);
    ASSERT_NE(rightValue, nullptr);
    ASSERT_EQ(leftValue->value, "true");
    ASSERT_EQ(rightValue->value, "false");
}

TEST(ParserTest, parseOrOperation) {
    Lexer lexer("var result: bool = true || false;");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::VariableDeclaration);

    const auto var = static_cast<ASTVariableDeclaration*>(program->body.front().get());
    ASSERT_EQ(var->name, "result");
    ASSERT_EQ(var->varType, ASTValueType::Bool);

    const auto operation = static_cast<ASTOrOperation*>(var->value.get());
    ASSERT_NE(operation, nullptr);

    const auto leftValue = static_cast<ASTValue*>(operation->left.get());
    const auto rightValue = static_cast<ASTValue*>(operation->right.get());

    ASSERT_NE(leftValue, nullptr);
    ASSERT_NE(rightValue, nullptr);
    ASSERT_EQ(leftValue->value, "true");
    ASSERT_EQ(rightValue->value, "false");
}

TEST(ParserTest, parsePrintExpressionWithParentheses) {
    Lexer lexer("println((1 + 2) * 5);");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::Print);

    const auto printStmt = static_cast<ASTPrint*>(program->body.front().get());
    ASSERT_TRUE(printStmt);

    const auto operation = static_cast<ASTBinaryOperation*>(printStmt->expression.get());
    ASSERT_EQ(operation->op, "*");

    const auto leftExpr = static_cast<ASTBinaryOperation*>(operation->left.get());
    ASSERT_NE(leftExpr, nullptr);
    ASSERT_EQ(leftExpr->op, "+");

    const auto rightExpr = static_cast<ASTValue*>(leftExpr->right.get());
    ASSERT_NE(rightExpr, nullptr);
    ASSERT_EQ(rightExpr->value, "2");

    const auto leftValue = static_cast<ASTValue*>(leftExpr->left.get());
    ASSERT_NE(leftValue, nullptr);
    ASSERT_EQ(leftValue->value, "1");

    const auto multiplier = static_cast<ASTValue*>(operation->right.get());
    ASSERT_NE(multiplier, nullptr);
    ASSERT_EQ(multiplier->value, "5");
}

TEST(ParserTest, parsePrintNestedParentheses) {
    Lexer lexer("println(((3 + 4) * (2 + 1)));");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::Print);

    const auto printStmt = static_cast<ASTPrint*>(program->body.front().get());
    ASSERT_TRUE(printStmt);

    const auto operation = static_cast<ASTBinaryOperation*>(printStmt->expression.get());
    ASSERT_EQ(operation->op, "*");

    const auto leftExpr = static_cast<ASTBinaryOperation*>(operation->left.get());
    ASSERT_NE(leftExpr, nullptr);
    ASSERT_EQ(leftExpr->op, "+");

    const auto rightExpr = static_cast<ASTBinaryOperation*>(operation->right.get());
    ASSERT_NE(rightExpr, nullptr);
    ASSERT_EQ(rightExpr->op, "+");

    const auto leftValue = static_cast<ASTValue*>(leftExpr->left.get());
    ASSERT_NE(leftValue, nullptr);
    ASSERT_EQ(leftValue->value, "3");

    const auto rightValue = static_cast<ASTValue*>(leftExpr->right.get());
    ASSERT_NE(rightValue, nullptr);
    ASSERT_EQ(rightValue->value, "4");

    const auto nestedLeftValue = static_cast<ASTValue*>(rightExpr->left.get());
    ASSERT_NE(nestedLeftValue, nullptr);
    ASSERT_EQ(nestedLeftValue->value, "2");

    const auto nestedRightValue = static_cast<ASTValue*>(rightExpr->right.get());
    ASSERT_NE(nestedRightValue, nullptr);
    ASSERT_EQ(nestedRightValue->value, "1");
}

TEST(ASTFStringTest, BasicFString) {
    Lexer lexer("var greeting: string = \"Hello, {name}!\";");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::VariableDeclaration);

    const auto varDecl = static_cast<ASTVariableDeclaration*>(program->body.front().get());
    ASSERT_EQ(varDecl->name, "greeting");
    ASSERT_EQ(varDecl->varType, ASTValueType::String);

    const auto fstring = static_cast<ASTFString*>(varDecl->value.get());
    ASSERT_NE(fstring, nullptr);
    ASSERT_EQ(fstring->value, "Hello, {name}!");
    ASSERT_EQ(fstring->line, 1);
}

TEST(ASTFStringTest, PrintFString) {
    Lexer lexer("println(\"Welcome, {name}!\");");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::Print);

    const auto print = static_cast<ASTPrint*>(program->body.front().get());
    const auto fstring = static_cast<ASTFString*>(print->expression.get());
    ASSERT_NE(fstring, nullptr);
    ASSERT_EQ(fstring->value, "Welcome, {name}!");
    ASSERT_EQ(fstring->line, 1);
}

TEST(ParserTest, parseSimpleComparison) {
    Lexer lexer("var isEqual: bool = a == b;");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::VariableDeclaration);

    const auto var = static_cast<ASTVariableDeclaration*>(program->body.front().get());
    ASSERT_EQ(var->name, "isEqual");
    ASSERT_EQ(var->varType, ASTValueType::Bool);

    const auto comparison = static_cast<ASTBinaryOperation*>(var->value.get());
    ASSERT_NE(comparison, nullptr);
    ASSERT_EQ(comparison->op, "==");

    const auto leftVar = static_cast<ASTVariable*>(comparison->left.get());
    const auto rightVar = static_cast<ASTVariable*>(comparison->right.get());
    ASSERT_NE(leftVar, nullptr);
    ASSERT_NE(rightVar, nullptr);
    ASSERT_EQ(leftVar->name, "a");
    ASSERT_EQ(rightVar->name, "b");
}

TEST(ParserTest, parseGreaterThanComparison) {
    Lexer lexer("if (a > b) println(\"a is greater than b\");");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_EQ(program->type, ASTType::Program);
    ASSERT_EQ(program->body.size(), 1);
    ASSERT_EQ(program->body.front()->type, ASTType::Condition);

    const auto condition = static_cast<ASTCondition*>(program->body.front().get());
    const auto conditionExpression = static_cast<ASTBinaryOperation*>(condition->expression.get());

    ASSERT_NE(condition, nullptr);
    ASSERT_EQ(conditionExpression->op, ">");

    ASSERT_EQ(condition->body.size(), 1);
    const auto printStmt = static_cast<ASTPrint*>(condition->body.front().get());
    const auto printValue = static_cast<ASTValue*>(printStmt->expression.get());

    ASSERT_NE(printStmt, nullptr);
    ASSERT_EQ(printValue->value, "a is greater than b");
}

TEST(ParserTest, parseNegativeBoolThrowsException) {
    Lexer lexer("var a: bool = -true;");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    try {
        auto program = parser.parse();
        FAIL() << "Expected ZynkError but none was thrown";
    }
    catch (const ZynkError& e) {
        ASSERT_EQ(e.base_type, ZynkErrorType::SyntaxError);
    }
    catch (...) {
        FAIL() << "Expected ZynkError, but caught different exception";
    }
}

TEST(ParserTest, parseIfElseStatementWithSyntaxError) {
    Lexer lexer("if (a > b) { println(10) else { println(20); }"); // Missing semicolon
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    try {
        parser.parse();
        FAIL() << "Expected ZynkError thrown.";
    }
    catch (const ZynkError& error) {
        ASSERT_EQ(error.base_type, ZynkErrorType::SyntaxError);
    }
    catch (const std::exception& error) {
        FAIL() << "Unexpected exception type: " << error.what();
    }
}

TEST(ParserTest, ShouldThrowSyntaxError) {
    Lexer lexer("def main()\n}"); // Missing '{'
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    try {
        parser.parse();
        FAIL() << "Expected ZynkError thrown.";
    }
    catch (const ZynkError& error) {
        ASSERT_EQ(error.base_type, ZynkErrorType::SyntaxError);
    }
    catch (const std::exception& error) {
        FAIL() << "Unexpected exception type: " << error.what();
    }
}

TEST(ParserTest, ShouldThrowInvalidTypeError) {
    Lexer lexer("var a: abc = 10;"); // Invalid type 'abc'
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    try {
        parser.parse();
        FAIL() << "Expected ZynkError thrown.";
    }
    catch (const ZynkError& error) {
        ASSERT_EQ(error.base_type, ZynkErrorType::TypeError);
    }
    catch (const std::exception& error) {
        FAIL() << "Unexpected exception type: " << error.what();
    }
}

TEST(ParserTest, parseFunctionDeclarationWithMissingBracket) {
    Lexer lexer("def main({\n    println(10);"); // Missing closing bracket
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    try {
        parser.parse();
        FAIL() << "Expected ZynkError thrown.";
    }
    catch (const ZynkError& error) {
        ASSERT_EQ(error.base_type, ZynkErrorType::SyntaxError);
    }
    catch (const std::exception& error) {
        FAIL() << "Unexpected exception type: " << error.what();
    }
}

TEST(ParserTest, parseFunctionWithInvalidExpression) {
    Lexer lexer("def main() -> null {\n    println(10 + ;\n}"); // Invalid expression with missing operand
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    try {
        parser.parse();
        FAIL() << "Expected ZynkError thrown.";
    }
    catch (const ZynkError& error) {
        ASSERT_EQ(error.base_type, ZynkErrorType::ExpressionError);
    }
    catch (const std::exception& error) {
        FAIL() << "Unexpected exception type: " << error.what();
    }
}

TEST(ParserTest, parseFunctionCallWithExtraTokens) {
    Lexer lexer("main(10);"); // Extra arg in function call
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    try {
        parser.parse();
        FAIL() << "Expected ZynkError thrown.";
    }
    catch (const ZynkError& error) {
        ASSERT_EQ(error.base_type, ZynkErrorType::SyntaxError);
    }
    catch (const std::exception& error) {
        FAIL() << "Unexpected exception type: " << error.what();
    }
}

TEST(ParserTest, parseVariableDeclarationWithMissingColon) {
    Lexer lexer("var a int = 10;"); // Missing colon
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    try {
        parser.parse();
        FAIL() << "Expected ZynkError thrown.";
    }
    catch (const ZynkError& error) {
        ASSERT_EQ(error.base_type, ZynkErrorType::SyntaxError);
    }
    catch (const std::exception& error) {
        FAIL() << "Unexpected exception type: " << error.what();
    }
}

TEST(ParserTest, parseExpressionWithInvalidOperator) {
    Lexer lexer("var a: int = 5 $ 10;"); // Invalid operator '$'
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    try {
        parser.parse();
        FAIL() << "Expected ZynkError thrown.";
    }
    catch (const ZynkError& error) {
        ASSERT_EQ(error.base_type, ZynkErrorType::SyntaxError);
    }
    catch (const std::exception& error) {
        FAIL() << "Unexpected exception type: " << error.what();
    }
}

TEST(ParserTest, parseStringWithMissingClosingQuote) {
    Lexer lexer("var a: string = \"Unclosed string;");
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    try {
        parser.parse();
        FAIL() << "Expected ZynkError thrown.";
    }
    catch (const ZynkError& error) {
        ASSERT_EQ(error.base_type, ZynkErrorType::ExpressionError);
    }
    catch (const std::exception& error) {
        FAIL() << "Unexpected exception type: " << error.what();
    }
}