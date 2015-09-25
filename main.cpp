
#include "LOLexer.hxx"
#include "LOParser.hxx"
#include "LOScript.hxx"
#include "LOLModule.hxx"

#include "LOHLBasic.hxx"
#include "LOHLASTVisitor.hxx"

#include <memory>

#include <iostream>

//const unsigned char testcode[] = "var a : int32, b : double;";
const unsigned char testcode[] = ""
"class test {"
"   a : double;"
"   c : test;"
"}"
""
"extern function putchar(c : int32) : void;"
"extern function fabs(arg : double) : double;"
""
"   function main($a? : int32, b : int32) : int32 {"
        "var t : double;"
        "putchar(233);"
        "fabs(t);"
        "return fabs(t);"
"   }"
        ""
        "function t() : int32 {"
            "if (1) {"
            "   main(1, 3);"
            "}"
            "if (0) {"
        "       main(2, 4);"
        "   }"
        "}"
"";

int main() {
    using namespace Olivia;
    LOLexer lexer;
    lexer.dataptr = &testcode[0];
    lexer.advance();
    LOParser parser;
    parser.lexer = &lexer;
    parser.lex();
    std::shared_ptr<AST::NodeScript> root = std::make_shared<AST::NodeScript>();
    std::shared_ptr<LOScript> script = std::make_shared<LOScript>();
    std::shared_ptr<LOLModule> module = std::make_shared<LOLModule>("module_test", script);

    std::shared_ptr<HL::HLScript> hlscript = std::make_shared<HL::HLScript>(script);
    std::shared_ptr<HL::HLBlockBuilder> builder = std::make_shared<HL::HLBlockBuilder>(hlscript);
    std::shared_ptr<HL::HLEmitterVisitor> visitor = std::make_shared<HL::HLEmitterVisitor>(builder);

    while (!lexer.reached_eof) {
        printf("parsing ...\n");
        std::shared_ptr<AST::Node> t = parser.parse_statement(root);
        t->dump();

        switch (t->kind()) {
            case AST::NDeclarationClass:
                script->addClass(t);
                break;
            case AST::NDeclarationFunction:
                script->addGlobalFunction(std::static_pointer_cast<AST::NodeDeclarationFunction>(t));
                break;
            case AST::NDeclarationExternFunction:
                script->addExternFunction(std::static_pointer_cast<AST::NodeDeclarationExternFunction>(t));
                break;
            case AST::NDeclarationVar:
                break;
            default:
                break;
        }

//        auto ir = t->generate_code(module.get());
        t->accept(visitor.get());
//        if (ir) {
//            printf("Generated code:\n");
//            ir->dump();
//        } else {
//            printf("generation not defined!\n");
//        }
    }
    script->finishupDeclarations();

    std::cout << hlscript->toString() << std::endl;

    return 0;
}
