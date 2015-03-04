#include "hlsl_printer.h"

#include <cassert>
#include <utils/indentation.h>
#include <ast/node.h>

namespace AST
{

    void HLSLPrinter::Visit( const Node & node )
    {
        assert( !"Unsupported node type, implement in base class" );
    }

    void HLSLPrinter::Visit( const TranslationUnit & translation_unit )
    {
        {
            std::vector< Base::ObjectRef<GlobalDeclaration> >::const_iterator it, end;
            it = translation_unit.m_GlobalDeclarationTable.cbegin();
            end = translation_unit.m_GlobalDeclarationTable.cend();

            for( ;it != end; ++it )
            {
                (*it)->Visit( *this );
            }
        }

        {
            std::vector< Base::ObjectRef<Technique> >::const_iterator it, end;
            it = translation_unit.m_TechniqueTable.cbegin();
            end = translation_unit.m_TechniqueTable.cend();

            for( ;it != end; ++it )
            {
                (*it)->Visit( *this );
            }
        }
    }

    void HLSLPrinter::Visit( const VariableDeclaration & variable_declaration )
    {
        {
            std::vector< Base::ObjectRef<StorageClass> >::const_iterator it, end;
            it = variable_declaration.m_StorageClass.cbegin();
            end = variable_declaration.m_StorageClass.cend();
            bool first = true;
            for( ;it != end; ++it )
            {
                if(!first) m_Stream << "," << endl_ind;
                first = false;
                (*it)->Visit( *this );
            }
        }

        {
            std::vector< Base::ObjectRef<TypeModifier> >::const_iterator it, end;
            it = variable_declaration.m_TypeModifier.cbegin();
            end = variable_declaration.m_TypeModifier.cend();

            for( ;it != end; ++it )
            {
                (*it)->Visit( *this );
            }
        }

        m_Stream << variable_declaration.m_Type->m_Name;

        {
            std::vector< Base::ObjectRef<VariableDeclarationBody> >::const_iterator it, end;
            it = variable_declaration.m_BodyTable.cbegin();
            end = variable_declaration.m_BodyTable.cend();

            m_Stream << inc_ind << endl_ind;

            for( ;it != end; ++it )
            {
                (*it)->Visit( *this );
            }

            m_Stream << dec_ind;
        }

        m_Stream << ";" << endl_ind;
    }

    void HLSLPrinter::Visit( const IntrinsicType & type )
    {
        m_Stream << type.m_Name;
    }

    void HLSLPrinter::Visit( const UserDefinedType & type )
    {
        m_Stream << type.m_Name;
    }

    void HLSLPrinter::Visit( const SamplerType & type )
    {
        m_Stream << type.m_Name;
    }

    void HLSLPrinter::Visit( const TypeModifier & modifier )
    {
        m_Stream << modifier.m_Value;
    }

    void HLSLPrinter::Visit( const StorageClass & storage_class )
    {
        m_Stream << storage_class.m_Value;
    }

    void HLSLPrinter::Visit( const VariableDeclarationBody & body )
    {
        m_Stream << body.m_Name;

        if( body.m_ArraySize )
        {
            m_Stream << "[" << body.m_ArraySize << "]";
        }

        if( !body.m_Semantic.empty() )
        {
            m_Stream << " : " << body.m_Semantic;
        }

        if( body.m_Annotations )
        {
            body.m_Annotations->Visit( *this );
        }

        if( body.m_InitialValue )
        {
            m_Stream << " = ";

            body.m_InitialValue->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( const TextureDeclaration & declaration )
    {
        m_Stream << declaration.m_Type << " " << declaration.m_Name;

        if( !declaration.m_Semantic.empty() )
        {
            m_Stream << " : " << declaration.m_Semantic;
        }

        if( declaration.m_Annotations )
        {
            declaration.m_Annotations->Visit( *this );
        }

        m_Stream << ";" << endl_ind;
    }

    void HLSLPrinter::Visit( const SamplerDeclaration & declaration )
    {
        m_Stream << declaration.m_Type << " "
            << declaration.m_Name << endl_ind
            << "{" << inc_ind << endl_ind;

        {
            std::vector< Base::ObjectRef<SamplerBody> >::const_iterator it, end;
            it = declaration.m_BodyTable.cbegin();
            end = declaration.m_BodyTable.cend();
            for(; it!=end; ++it )
            {
                (*it)->Visit( *this );
            }

        }

        m_Stream << dec_ind << endl_ind << "}" << endl_ind;
    }

    void HLSLPrinter::Visit( const SamplerBody & body )
    {
        if( body.m_Name == "texture" )
        {
            m_Stream << "texture=<" << body.m_Value << ">";
        }
        else
        {
            m_Stream << body.m_Name << " = " << body.m_Value;
        }

        m_Stream << ";" << endl_ind;

    }

    void HLSLPrinter::Visit( const StructDefinition & definition )
    {
        m_Stream << " struct " << definition.m_Name << endl_ind;
        m_Stream << "{" << inc_ind << endl_ind;

        {
            std::vector< StructDefinition::Member >::const_iterator it, end;
            it = definition.m_MemberTable.cbegin();
            end = definition.m_MemberTable.cend();

            for(; it!=end; ++it )
            {
                const StructDefinition::Member
                    & member = (*it);

                if( !member.m_InterpolationModifier.empty() )
                {
                    m_Stream << member.m_InterpolationModifier << " ";
                }

                member.m_Type->Visit( *this );

                m_Stream << " " << member.m_Name;

                if( !member.m_Semantic.empty() )
                {
                    m_Stream << " : " <<member.m_Semantic;
                }
            }
        }

    }

    void HLSLPrinter::Visit( const FunctionDeclaration & declaration )
    {
        {
            std::vector< Base::ObjectRef<StorageClass> >::const_iterator it, end;
            it = declaration.m_StorageClassTable.cbegin();
            end = declaration.m_StorageClassTable.cend();

            for(; it!=end; ++it )
            {
                (*it)->Visit( *this );
            }
        }

        if( declaration.m_Type )
        {
            declaration.m_Type->Visit( *this );
        }
        else
        {
            m_Stream << "void";
        }
        m_Stream << " " << declaration.m_Name << "(";
        if( declaration.m_ArgumentList )
        {
            declaration.m_ArgumentList->Visit( * this );
        }
        m_Stream << ")";

        if( !declaration.m_Semantic.empty() )
        {
            m_Stream << " : " << declaration.m_Semantic;
        }

        m_Stream << endl_ind << "{" << inc_ind << endl_ind;

        {
            std::vector< Base::ObjectRef<Statement> >::const_iterator it, end;
            it = declaration.m_StatementTable.cbegin();
            end = declaration.m_StatementTable.cend();

            for(; it!=end; ++it )
            {
                (*it)->Visit( *this );
            }
        }

        m_Stream << dec_ind << endl_ind << "}" << endl_ind;
    }

    void HLSLPrinter::Visit( const ArgumentList & list )
    {
        bool first = true;

        {
            std::vector< Base::ObjectRef<Argument> >::const_iterator it, end;
            it = list.m_ArgumentTable.cbegin();
            end = list.m_ArgumentTable.cend();

            for(; it!=end; ++it )
            {
                if( !first ) m_Stream << ", ";
                first = false;
                (*it)->Visit( *this );
            }
        }
    }

    void HLSLPrinter::Visit( const Argument & argument )
    {
        if( !argument.m_InputModifier.empty() )
        {
            m_Stream << argument.m_InputModifier << " ";
        }

        if( argument.m_TypeModifier )
        {
            argument.m_TypeModifier->Visit( *this );
        }

        m_Stream << argument.m_Type->m_Name << " " << argument.m_Name;

        if( !argument.m_Semantic.empty() )
        {
            m_Stream << " : " << argument.m_Semantic;
        }

        if( !argument.m_InterpolationModifier.empty() )
        {
            m_Stream << " " << argument.m_InterpolationModifier;
        }

        if( argument.m_InitialValue )
        {
            m_Stream << " = ";

            argument.m_InitialValue->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( const LiteralExpression & expression )
    {
        m_Stream << expression.m_Value;
    }

    void HLSLPrinter::Visit( const VariableExpression & expression )
    {
        m_Stream << expression.m_Name;

        if( expression.m_SubscriptExpression )
        {
            m_Stream << '[';
            expression.m_SubscriptExpression->Visit( *this );
            m_Stream << ']';
        }
    }

    void HLSLPrinter::Visit( const UnaryOperationExpression & expression )
    {
        m_Stream << expression.m_Operation << "( ";
        expression.m_Expression->Visit( *this );
        m_Stream << " )";
    }

    void HLSLPrinter::Visit( const BinaryOperationExpression & expression )
    {
        m_Stream << "( ";
        expression.m_LeftExpression->Visit( *this );
        m_Stream << " ) " << expression.m_Operation << " ( ";
        expression.m_RightExpression->Visit( *this );
        m_Stream << " )";
    }

    void HLSLPrinter::Visit( const CallExpression & expression )
    {
        m_Stream << expression.m_Name << "(";

        if( expression.m_ArgumentExpressionList )
        {
            expression.m_ArgumentExpressionList->Visit( *this );
        }

        m_Stream << ")";
    }

    void HLSLPrinter::Visit( const ArgumentExpressionList & list )
    {
        size_t
            index,
            count;

        count = list.m_ExpressionList.size();

        for( index = 0; index < count; ++index )
        {
            if( index != 0 ) m_Stream << ", ";
            list.m_ExpressionList[ index ]->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( const Swizzle & swizzle )
    {
        m_Stream << "." << swizzle.m_Swizzle;
    }

    void HLSLPrinter::Visit( const PostfixSuffixCall & postfix_suffix )
    {
        m_Stream << ".";
        postfix_suffix.m_CallExpression->Visit( *this );

        if( postfix_suffix.m_Suffix )
        {
            postfix_suffix.m_Suffix->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( const PostfixSuffixVariable & postfix_suffix )
    {
        m_Stream << ".";
        postfix_suffix.m_VariableExpression->Visit( *this );

        if( postfix_suffix.m_Suffix )
        {
            postfix_suffix.m_Suffix->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( const ConstructorExpression & expression )
    {
        expression.m_Type->Visit( *this );
        m_Stream << "(";

        if( expression.m_ArgumentExpressionList )
        {
            expression.m_ArgumentExpressionList->Visit( *this );
        }

        m_Stream << ")";
    }

    void HLSLPrinter::Visit( const ConditionalExpression & expression )
    {
        //:TODO: generate parenthesis only when required ( using operator precedence )
        m_Stream << "( ";
        expression.m_Condition->Visit( *this );
        m_Stream << " ) ? ( ";
        expression.m_IfTrue->Visit( *this );
        m_Stream << " ) : ( ";
        expression.m_IfFalse->Visit( *this );
        m_Stream << " )";
    }

    void HLSLPrinter::Visit( const LValueExpression & expression )
    {
        expression.m_VariableExpression->Visit( *this );

        if( expression.m_Suffix )
        {
            expression.m_Suffix->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( const PreModifyExpression & expression )
    {
        m_Stream << expression.m_Operator;
        expression.m_Expression->Visit( *this );
    }

    void HLSLPrinter::Visit( const PostModifyExpression & expression )
    {
        expression.m_Expression->Visit( *this );
        m_Stream << expression.m_Operator;
    }

    void HLSLPrinter::Visit( const CastExpression & expression )
    {
        m_Stream << "( ";
        expression.m_Type->Visit( *this );

        if( expression.m_ArraySize != -1 )
        {
            m_Stream << "[" << expression.m_ArraySize << "]";
        }

        m_Stream <<" )( ";

        expression.m_Expression->Visit( *this );

        m_Stream << " )";
    }

    void HLSLPrinter::Visit( const AssignmentExpression & expression )
    {
        expression.m_LValueExpression->Visit( *this );
        m_Stream << " " << expression.m_Operator << " ";
        expression.m_Expression->Visit( *this );
    }

    void HLSLPrinter::Visit( const PostfixExpression & expression )
    {
        expression.m_Expression->Visit( *this );

        if( expression.m_Suffix )
        {
            expression.m_Suffix->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( const ReturnStatement & statement )
    {
        m_Stream << "return";

        if( statement.m_Expression )
        {
            m_Stream << " ";
            statement.m_Expression->Visit( *this );
        }

        m_Stream << ";" << endl_ind;
    }

    void HLSLPrinter::Visit( const BreakStatement & statement )
    {
        m_Stream << "break;" << endl_ind;
    }

    void HLSLPrinter::Visit( const ContinueStatement & statement )
    {
        m_Stream << "continue;" << endl_ind;
    }

    void HLSLPrinter::Visit( const DiscardStatement & statement )
    {
        m_Stream << "discard;" << endl_ind;
    }

    void HLSLPrinter::Visit( const EmptyStatement & statement )
    {
        m_Stream << ";" << endl_ind;
    }

    void HLSLPrinter::Visit( const ExpressionStatement & statement )
    {
        statement.m_Expression->Visit( *this );
        m_Stream << ";" << endl_ind;
    }

    void HLSLPrinter::Visit( const IfStatement & statement )
    {
        m_Stream << "if( ";
        statement.m_Condition->Visit( *this );
        m_Stream << " ) ";
        statement.m_ThenStatement->Visit( *this );

        if( statement.m_ElseStatement)
        {
            m_Stream << "else ";
            statement.m_ElseStatement->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( const WhileStatement & statement )
    {
        m_Stream << "while( ";
        statement.m_Condition->Visit( *this );
        m_Stream << " ) ";
        statement.m_Statement->Visit( *this );
    }

    void HLSLPrinter::Visit( const DoWhileStatement & statement )
    {
        m_Stream << "do ";
        statement.m_Statement->Visit( *this );
        m_Stream << "while( ";
        statement.m_Condition->Visit( *this );
        m_Stream << " );\n";
    }

    void HLSLPrinter::Visit( const BlockStatement & statement )
    {
        if( statement.m_StatementTable.empty() )
        {
            m_Stream << "{}" << endl_ind;
        }
        else
        {
            m_Stream << "{" << inc_ind << endl_ind;

            std::vector< Base::ObjectRef<Statement> >::const_iterator
                it, end = statement.m_StatementTable.cend();

            for ( it = statement.m_StatementTable.cbegin(); it != end; ++it )
            {
                (*it)->Visit( *this );
            }

            m_Stream << dec_ind << endl_ind << "}" << endl_ind;
        }
    }

    void HLSLPrinter::Visit( const AssignmentStatement & statement )
    {
        statement.m_Expression->Visit( *this );
        m_Stream << ";" << endl_ind;
    }

    void HLSLPrinter::Visit( const VariableDeclarationStatement & statement )
    {
        {
            std::vector< Base::ObjectRef<StorageClass> >::const_iterator it, end;
            it = statement.m_StorageClass.cbegin();
            end = statement.m_StorageClass.cend();
            bool first = true;
            for( ;it != end; ++it )
            {
                if(!first) m_Stream << "," << endl_ind;
                first = false;
                (*it)->Visit( *this );
            }
        }

        {
            std::vector< Base::ObjectRef<TypeModifier> >::const_iterator it, end;
            it = statement.m_TypeModifier.cbegin();
            end = statement.m_TypeModifier.cend();

            for( ;it != end; ++it )
            {
                (*it)->Visit( *this );
            }
        }

        m_Stream << statement.m_Type->m_Name;

        {
            std::vector< Base::ObjectRef<VariableDeclarationBody> >::const_iterator it, end;
            it = statement.m_BodyTable.cbegin();
            end = statement.m_BodyTable.cend();

            m_Stream << inc_ind << endl_ind;

            for( ;it != end; ++it )
            {
                (*it)->Visit( *this );
            }

            m_Stream << dec_ind;
        }

        m_Stream << ";" << endl_ind;
    }

}
