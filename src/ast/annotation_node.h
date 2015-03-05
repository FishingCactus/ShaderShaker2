#ifndef ANNOTATION_NODE_H
    #define ANNOTATION_NODE_H


    namespace AST
    {
        struct AnnotationEntry;

        struct Annotations : Node
        {
            AST_HandleVisitor()

            void AddEntry( AnnotationEntry * entry ) { m_AnnotationTable.emplace_back( entry ); }

            virtual Annotations * Clone() const override;

            std::vector< Base::ObjectRef< AnnotationEntry> >
                m_AnnotationTable;
        };

        struct AnnotationEntry : Node
        {
            AST_HandleVisitor()

            AnnotationEntry() {}
            AnnotationEntry( const std::string & type, const std::string & name, const std::string & value ) :
                m_Type( type ), m_Name( name ), m_Value( value ) {}

            virtual AnnotationEntry * Clone() const override;

            std::string
                m_Type,
                m_Name,
                m_Value;
        };
    }

#endif