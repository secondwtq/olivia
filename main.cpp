
#include "LOLexer.hxx"
#include "LOParser.hxx"
#include "LOScript.hxx"

#include <memory>

//const unsigned char testcode[] = "var a : int32, b : double;";
const unsigned char testcode[] = ""
"class test {"
"   a : double;"
"   c : test;"
"}"
""
"extern function putchar(c : int32) : void;"
""
"   var t : test; "
"   function main(argc : int32, argv : string) : int32 {"
"       var t1 : test; "
"   }"
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

    while (!lexer.reached_eof) {
        printf("parsing ...\n");
        std::shared_ptr<AST::Node> t = parser.parse_statement(root);
        t->dump();

        switch (t->kind()) {
            case AST::NDeclarationClass:
                script->addClass(t);
                break;
            case AST::NDeclarationVar:
                break;
            default:
                break;
        }
    }
    script->finishupDeclarations();
    return 0;
}
