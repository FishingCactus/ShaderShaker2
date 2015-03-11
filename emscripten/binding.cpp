#include <emscripten/bind.h>
#include <hlsl_parser/hlsl.h>
#include <ast/node.h>


namespace emscripten
{
    template<typename PointeeType>
    struct smart_ptr_trait<Base::ObjectRef<PointeeType>>
    {
        typedef Base::ObjectRef<PointeeType> PointerType;
        typedef PointeeType element_type;

        static element_type* get(const PointerType& ptr)
        {
            return const_cast<PointeeType*>( &*ptr );
        }

        static sharing_policy get_sharing_policy()
        {
            return sharing_policy::INTRUSIVE;
        }

        static Base::ObjectRef<PointeeType>* share(PointeeType* p, internal::EM_VAL v)
        {
            assert( "not implemented" );
            return 0;
        }

        static PointerType* construct_null()
        {
            return new PointerType;
        }
    };
}

EMSCRIPTEN_BINDINGS(ShaderShaker2)
{
    emscripten::function("ParseHLSL", &HLSL::ParseHLSL );

    emscripten::class_<AST::TranslationUnit>("TranslationUnit")
        .smart_ptr<Base::ObjectRef<AST::TranslationUnit>>("TranslationUnitRef");
}
