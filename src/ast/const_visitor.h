#ifndef CONST_VISITOR_H
    #define CONST_VISITOR_H

    #include "visitor_base.h"

    namespace AST
    {
        typedef VisitorBase<true> ConstVisitor;
    }

#endif