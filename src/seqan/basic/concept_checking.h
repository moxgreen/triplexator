// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2006-2012, Knut Reinert, FU Berlin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Knut Reinert or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL KNUT REINERT OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================
// Author: Manuel Holtgrewe <manuel.holtgrewe@fu-berlin.de>
// ==========================================================================
// A minimal subset of the Boost Concept Checking Library.  A lot of the code
// in the BCCL deals with support of non-conforming compilers and we cut this
// away.  The code here has been adjusted to work with the compilers supported
// by SeqAn and be as simple as possible while still creating useful compiler
// errors.
// ==========================================================================

// SEQAN_NO_GENERATED_FORWARDS

#ifndef CORE_INCLUDE_SEQAN_BASIC_CONCEPT_CHECKING_H_
#define CORE_INCLUDE_SEQAN_BASIC_CONCEPT_CHECKING_H_

namespace seqan {

// ---------------------------------------------------------------------------
// ==> boost/static_assert.hpp <==
// ---------------------------------------------------------------------------

//  (C) Copyright John Maddock 2000.
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/static_assert for documentation.

#ifdef SEQAN_CXX11_STANDARD
#  define SEQAN_STATIC_ASSERT_MSG( B, Msg ) static_assert(B, Msg)
#else
#  define SEQAN_STATIC_ASSERT_MSG( B, Msg ) SEQAN_STATIC_ASSERT( B )
#endif

//
// If the compiler issues warnings about old C style casts,
// then enable this:
//
//#if defined(__GNUC__) && ((__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 4)))
//#  define BOOST_STATIC_ASSERT_BOOL_CAST( x ) ((x) == 0 ? false : true)
//#else
#  define SEQAN_STATIC_ASSERT_BOOL_CAST(x) (bool)(x)
//#endif

#ifdef SEQAN_CXX11_STANDARD
#  define SEQAN_STATIC_ASSERT( B ) static_assert(B, #B)
#else

// HP aCC cannot deal with missing names for template value parameters
template <bool x> struct STATIC_ASSERTION_FAILURE;

template <> struct STATIC_ASSERTION_FAILURE<true> { enum { value = 1 }; };

// HP aCC cannot deal with missing names for template value parameters
template<int x> struct static_assert_test{};

//
// Implicit instantiation requires that all member declarations be
// instantiated, but that the definitions are *not* instantiated.
//
// It's not particularly clear how this applies to enum's or typedefs;
// both are described as declarations [7.1.3] and [7.2] in the standard,
// however some compilers use "delayed evaluation" of one or more of
// these when implicitly instantiating templates.  We use typedef declarations
// by default, but try defining SEQAN_USE_ENUM_STATIC_ASSERT if the enum
// version gets better results from your compiler...
//
// Implementation:
// Both of these versions rely on sizeof(incomplete_type) generating an error
// message containing the name of the incomplete type.  We use
// "STATIC_ASSERTION_FAILURE" as the type name here to generate
// an eye catching error message.  The result of the sizeof expression is either
// used as an enum initialiser, or as a template argument depending which version
// is in use...
// Note that the argument to the assert is explicitly cast to bool using old-
// style casts: too many compilers currently have problems with static_cast
// when used inside integral constant expressions.
//
//#if !defined(SEQAN_BUGGY_INTEGRAL_CONSTANT_EXPRESSIONS)
/*
#if defined(SEQAN_MSVC) && (SEQAN_MSVC < 1300)
// __LINE__ macro broken when -ZI is used see Q199057
// fortunately MSVC ignores duplicate typedef's.
#define SEQAN_STATIC_ASSERT( B ) \
   typedef static_assert_test<\
      sizeof(STATIC_ASSERTION_FAILURE< (bool)( B ) >)\
      > seqan_static_assert_typedef_
#elif defined(SEQAN_MSVC)
*/
#if defined(PLATFORM_WINDOWS_VS)
#define SEQAN_STATIC_ASSERT( B ) \
   typedef static_assert_test<\
      sizeof(STATIC_ASSERTION_FAILURE< SEQAN_STATIC_ASSERT_BOOL_CAST ( B ) >)>\
         SEQAN_JOIN(seqan_static_assert_typedef_, __COUNTER__)
/*
#elif defined(SEQAN_INTEL_CXX_VERSION) || defined(SEQAN_SA_GCC_WORKAROUND)
// agurt 15/sep/02: a special care is needed to force Intel C++ issue an error 
// instead of warning in case of failure
# define SEQAN_STATIC_ASSERT( B ) \
    typedef char SEQAN_JOIN(seqan_static_assert_typedef_, __LINE__) \
        [ STATIC_ASSERTION_FAILURE< SEQAN_STATIC_ASSERT_BOOL_CAST( B ) >::value ]
#elif defined(__sgi)
// special version for SGI MIPSpro compiler
#define SEQAN_STATIC_ASSERT( B ) \
   SEQAN_STATIC_CONSTANT(bool, \
     SEQAN_JOIN(boost_static_assert_test_, __LINE__) = ( B )); \
   typedef static_assert_test<\
     sizeof(STATIC_ASSERTION_FAILURE< \
       SEQAN_JOIN(boost_static_assert_test_, __LINE__) >)>\
         SEQAN_JOIN(seqan_static_assert_typedef_, __LINE__)
#elif SEQAN_WORKAROUND(__MWERKS__, <= 0x3003)
// special version for CodeWarrior <= 8.x
#define SEQAN_STATIC_ASSERT( B ) \
   SEQAN_STATIC_CONSTANT(int, \
     SEQAN_JOIN(boost_static_assert_test_, __LINE__) = \
       sizeof(STATIC_ASSERTION_FAILURE< SEQAN_STATIC_ASSERT_BOOL_CAST( B ) >) )
*/
#else
// generic version
#define SEQAN_STATIC_ASSERT( B ) \
   typedef static_assert_test<\
      sizeof(STATIC_ASSERTION_FAILURE< SEQAN_STATIC_ASSERT_BOOL_CAST( B ) >)>\
         SEQAN_JOIN(seqan_static_assert_typedef_, __LINE__)
#endif
/*
#else
// alternative enum based implementation:
#define SEQAN_STATIC_ASSERT( B ) \
   enum { SEQAN_JOIN(boost_static_assert_enum_, __LINE__) \
      = sizeof(STATIC_ASSERTION_FAILURE< (bool)( B ) >) }
#endif
*/
#endif

// ---------------------------------------------------------------------------
// ==> boost/parameter/aux_/paranthesized_type.hpp <==
// ---------------------------------------------------------------------------

// Copyright David Abrahams 2006. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

template <class UnaryFunctionPointer>
struct unaryfunptr_arg_type;

template <class Arg>
struct unaryfunptr_arg_type<void(*)(Arg)>
{
    typedef Arg type; 
};

template <>
struct unaryfunptr_arg_type<void(*)(void)>
{
    typedef void type;
};

// ---------------------------------------------------------------------------
// ==> boost/concept_check/general.hpp <==
// ---------------------------------------------------------------------------

// Copyright David Abrahams 2006. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

namespace concept_checking
{
template <void(*)()> struct instantiate {};
}

template <class ModelFn> struct concept_check_;

template <class Model>
void concept_check_failed()
{
    ((Model*)0)->~Model();
}

template <class Model>
struct concept_check
{
    concept_checking::instantiate<concept_check_failed<Model> > x;
    enum { instantiate = 1 };
};

template <class Model>
struct concept_check_<void(*)(Model)>
        : concept_check<Model>
{};

#  define SEQAN_CONCEPT_ASSERT_FN( ModelFnPtr )             \
    typedef ::seqan::detail::instantiate<          \
    &::seqan::requirement_<ModelFnPtr>::failed>    \
      SEQAN_PP_CAT(seqan_concept_check,__LINE__)

// ---------------------------------------------------------------------------
// ==> boost/concept/assert.hpp <==
// ---------------------------------------------------------------------------

// Copyright David Abrahams 2006. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

/**
.Macro.SEQAN_CONCEPT_ASSERT
..cat:Concepts
..summary:Perform a concept check.
..signature:SEQAN_CONCEPT_ASSERT((concept))
..param.concept:Concept specialized with a the type that should be checked.
..remarks:This macro is a compile-time assertion and requires the concept specialized with the tested types to compile.
The check neither consumes memory nor running time.
The macro can be used at the beginning of a function or within a struct/class definition.
The checked concepts should be as restrictive and generic as possible to on the one hand 
cover all used functionality and on the other hand not limit the applicability of a function/class.
..example.code:
typedef typename Value<TContainer>::Type                TValue;
typedef typename Position<TContainer>::Type             TPosition;
typedef typename Difference<TContainer>::Type           TDifference;

SEQAN_CONCEPT_ASSERT((AlphabetConcept<TValue>));
SEQAN_CONCEPT_ASSERT((SignedIntegerConcept<TDifference>));
SEQAN_CONCEPT_ASSERT((UnsignedIntegerConcept<TSize>));
..include:seqan/basic.h
 */

// Usage, in class or function context:
//     SEQAN_CONCEPT_ASSERT((UnaryFunctionConcept<F,bool,int>));
# define SEQAN_CONCEPT_ASSERT(ModelInParens) \
    SEQAN_CONCEPT_ASSERT_FN(void(*)ModelInParens)

// usage.hpp

template <class Model>
struct usage_requirements
{
    ~usage_requirements() { ((Model*)0)->~Model(); }
};

/**
.Macro.SEQAN_CONCEPT_USAGE
..cat:Concepts
..summary:Define valid expressions.
..signature:SEQAN_CONCEPT_USAGE(name)
..param.name:Identifier of the concept defined with @Macro.SEQAN_CONCEPT@ or @Macro.SEQAN_CONCEPT_REFINE@.
..remarks:This macro should be used to introduce a block (enclosed with curly braces) of valid expressions within
a newly defined concept.
Valid expressions should test for available functions, operators and the correctness of return types.
Use helper functions, e.g. @Function.ignoreUnusedVariableWarning@, @Function.requireBooleanExpr@ and @Function.sameType@.
..example.code:
SEQAN_CONCEPT(EqualityComparable,(T))
{
    SEQAN_CONCEPT_USAGE(EqualityComparable) 
    {
        requireBooleanExpr(a == b);
        requireBooleanExpr(a != b);
    }
private:
    T a, b;
};
..see:Function.requireBooleanExpr
..see:Macro.SEQAN_CONCEPT
..see:Macro.SEQAN_CONCEPT_REFINE
..include:seqan/basic.h
 */
#define SEQAN_CONCEPT_USAGE(model)                                      \
    SEQAN_CONCEPT_ASSERT((seqan::usage_requirements<model>));           \
    ~model()

// ---------------------------------------------------------------------------
// ==> boost/concept/detail/has_constraints.hpp <==
// ---------------------------------------------------------------------------

// Copyright David Abrahams 2006. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

namespace detail {
  typedef char yes;
  typedef char (&no)[2];

  template <class Model, void (Model::*)()>
  struct wrap_constraints {};
    
  template <class Model>
  inline yes has_constraints_(Model*, wrap_constraints<Model,&Model::constraints>* = 0);
  inline no has_constraints_(...);
}

// This would be called "detail::has_constraints," but it has a strong
// tendency to show up in error messages.
template <class Model>
struct not_satisfied
{
    enum {value = sizeof( detail::has_constraints_((Model*)0) ) == sizeof(detail::yes) };
    typedef typename Eval<value>::Type Type;
};

// ---------------------------------------------------------------------------
// ==> boost/concept_check/detail/general.hpp <==
// ---------------------------------------------------------------------------

// Copyright David Abrahams 2006. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

template <class ModelFn>
struct requirement_;

namespace detail
{
  template <void(*)()> struct instantiate {};
}

template <class Model>
struct requirement
{
    static void failed() { ((Model*)0)->~Model(); }
};

struct failed {};

template <class Model>
struct requirement<failed ************ Model::************>
{
    static void failed() { ((Model*)0)->~Model(); }
};

template <class Model>
struct constraint
{
    static void failed() { ((Model*)0)->constraints(); }
};
  
template <class Model>
struct requirement_<void(*)(Model)>
        : IfC<not_satisfied<Model>::Type::VALUE, /* should be called "has_constraints", see above */
              constraint<Model>,
              requirement<failed ************ Model::************>
              >::Type
{};

#  define SEQAN_CONCEPT_ASSERT_FN( ModelFnPtr )             \
    typedef ::seqan::detail::instantiate<          \
    &::seqan::requirement_<ModelFnPtr>::failed>    \
      SEQAN_PP_CAT(seqan_concept_check,__LINE__)

// ---------------------------------------------------------------------------
// ==> boost/concept_check/detail/requires.hpp <==
// ---------------------------------------------------------------------------

// Copyright David Abrahams 2006. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Template for use in handwritten assertions
template <class Model, class More>
struct requires_ : More
{
    SEQAN_CONCEPT_ASSERT((Model));
};

// Template for use by macros, where models must be wrapped in parens.
// This isn't in namespace detail to keep extra cruft out of resulting
// error messages.

template <class ModelFn>
struct _requires_
{
    enum { value = 0 };
    SEQAN_CONCEPT_ASSERT_FN(ModelFn);
};

template <int check, class Result>
struct Requires_ : unaryfunptr_arg_type<Result>
{};

#  define SEQAN_CONCEPT_REQUIRES_(r,data,t) + (::seqan::_requires_<void(*)t>::value)

#if defined(NDEBUG)

# define SEQAN_CONCEPT_REQUIRES(models, result)                      \
    typename unaryfunptr_arg_type<void(*)result>::type

#else  // #if defined(NDEBUG)

# define SEQAN_CONCEPT_REQUIRES(models, result)                                        \
    typename ::seqan::Requires_<                                                       \
      (0 SEQAN_PP_SEQ_FOR_EACH(SEQAN_CONCEPT_REQUIRES_, ~, models)),                   \
      void(*)result                                                                 \
    >::type

#endif  // #if defined(NDEBUG)

// ---------------------------------------------------------------------------
// ==> boost/concept_check.hpp <==
// ---------------------------------------------------------------------------

//
// (C) Copyright Jeremy Siek 2000.
// Copyright 2002 The Trustees of Indiana University.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)    //
// Backward compatibility
//

template <class Model>
inline void functionRequires(Model* = 0)
{
    SEQAN_CONCEPT_ASSERT((Model));
}

/**
.Function.ignoreUnusedVariableWarning:
..summary:Removes unused variable warning.
..cat:Concepts
..signature:ignoreUnusedVariableWarning(x)
..param.x: Variable that causes the unused variable warning.
..remarks:It sometimes is necessary to define variables which are not further used, e.g. to check available assignment operators.
Use this functions to remove a compile warning that otherwise would be raised in this case.
..see:Macro.SEQAN_CONCEPT_USAGE
..include:seqan/basic.h
 */
template <class T> inline void ignoreUnusedVariableWarning(T const&) {}

// ---------------------------------------------------------------------------
// ==> boost/concept/detail/concept_def.hpp <==
// ---------------------------------------------------------------------------

// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// SEQAN_CONCEPT_REFINE added by David Weese

/**
.Macro.SEQAN_CONCEPT
..cat:Concepts
..summary:Defines a new concept.
..signature:SEQAN_CONCEPT(name, params)
..param.name:Concept identifier. Non-trivial concepts should have an identifier with a "Concept"-suffix.
..param.params:Template paramter list in parantheses, e.g. (T) or (T1)(T2).
Typically, template parameters are models, i.e. one or multiple classes that should be checked for fulfilling a concept.
...remarks:This is a sequence of the Boost Preprocessor Library, read @http://www.boost.org/doc/libs/1_47_0/libs/preprocessor/doc/index.html|more@.
..remarks:A concept is implemented as a template struct with name $name$ and arguments $params$.
The concept checking should be part of the struct definition.
Associated types should be checked via @Macro.SEQAN_CONCEPT_ASSERT@ and valid expressions in a function @Macro.SEQAN_CONCEPT_USAGE@, see below.
Variables used in valid expressions should be (private) struct members instead of local variables in member functions (read @http://www.boost.org/doc/libs/1_47_0/libs/concept_check/creating_concepts.htm|more@).
..example.code:
SEQAN_CONCEPT(Assignable,(T))
{
    SEQAN_CONCEPT_USAGE(Assignable) 
    {
        a = b;              // require assignment operator
        constConstraints(b);
    }
private:
    void constConstraints(const T& x) 
    {
        a = x;              // const required for argument to assignment
        ignoreUnusedVariableWarning(x);
    }
private:
    T a;
    T b;
};

SEQAN_CONCEPT(EqualityComparable,(T))
{
    SEQAN_CONCEPT_USAGE(EqualityComparable) 
    {
        requireBooleanExpr(a == b);
        requireBooleanExpr(a != b);
    }
private:
    T a, b;
};
..include:seqan/basic.h
 */

# define SEQAN_CONCEPT(name, params)                                            \
    template < SEQAN_PP_SEQ_FOR_EACH_I(SEQAN_CONCEPT_typename,~,params) >       \
    struct name

/**
.Macro.SEQAN_CONCEPT_REFINE
..cat:Concepts
..summary:Defines a new concept as a refinement of existing concepts.
..signature:SEQAN_CONCEPT_REFINE(name, params, refinedConcepts)
..param.name:Concept identifier. Non-trivial concepts should have an identifier with a "Concept"-suffix.
..param.params:Template paramter list in parantheses, e.g. (T) or (T1)(T2).
Typically, template parameters are models, i.e. one or multiple classes that should be checked for fulfilling a concept.
...remarks:This is a sequence of the Boost Preprocessor Library, read @http://www.boost.org/doc/libs/1_47_0/libs/preprocessor/doc/index.html|more@.
..param.refinedConcepts:Identifiers of concepts that are refined by the new concept.
...remarks:Refined concepts are implicitly integrated into the requirements of the new concept.
...remarks:This is a sequence of the Boost Preprocessor Library, read @http://www.boost.org/doc/libs/1_47_0/libs/preprocessor/doc/index.html|more@.
..remarks:A concept is implemented as a template struct with name $name$ and arguments $params$.
The struct inherits all refined concept structs.
The concept checking should be part of the struct definition.
For more information, see @Macro.SEQAN_CONCEPT@.
..example.code:
SEQAN_CONCEPT_REFINE(AlphabetConcept, (TValue), (Assignable)(DefaultConstructible)(CopyConstructible))
{
    TValue val, val2;

    SEQAN_CONCEPT_USAGE(AlphabetConcept)
    {
        assign(val, val2); 
    }
};
..include:seqan/basic.h
 */

# define SEQAN_CONCEPT_REFINE(name, params, refinedConcepts)                                        \
    template < SEQAN_PP_SEQ_FOR_EACH_I(SEQAN_CONCEPT_typename,~,params) >                           \
    struct name;                                                                                    \
                                                                                                    \
    template < SEQAN_PP_SEQ_FOR_EACH_I(SEQAN_CONCEPT_typename,~,params) >                           \
    struct Refines< name<SEQAN_PP_SEQ_ENUM(params)> >                                               \
    {                                                                                               \
        typedef                                                                                     \
            SEQAN_PP_SEQ_FOR_EACH_I(SEQAN_CONCEPT_LIST_prefix,params,refinedConcepts)               \
            SEQAN_PP_REPEAT(SEQAN_PP_SEQ_SIZE(refinedConcepts),SEQAN_CONCEPT_LIST_suffix,~) Type;   \
    };                                                                                              \
                                                                                                    \
    template < SEQAN_PP_SEQ_FOR_EACH_I(SEQAN_CONCEPT_typename,~,params) >                           \
    struct name:                                                                                    \
        SEQAN_PP_SEQ_FOR_EACH_I(SEQAN_CONCEPT_REFINE_superclass,params,refinedConcepts)

/**
.Macro.SEQAN_CONCEPT_IMPL
..cat:Concepts
..summary:Defines which concepts a model fulfills.
..signature:
template<> 
SEQAN_CONCEPT_IMPL(name, implementedConcepts)

template<typename T, int I>
SEQAN_CONCEPT_IMPL(name<T,I>, implementedConcepts)
..param.name:Model type, i.e. an identifier or an identifier with template arguments.
..param.implementedConcepts:Identifiers of concepts that are fulfilled by the model.
...remarks:This is a sequence of the Boost Preprocessor Library, read @http://www.boost.org/doc/libs/1_47_0/libs/preprocessor/doc/index.html|more@.
..remarks:The metafunction @Metafunction.Is@ can be used to determine whether a class models (fulfills) a concepts.
A model of a concept must pass the concept check via @Macro.SEQAN_CONCEPT_ASSERT@.
..example.code:
template <typename TValue, typename TSpec>
SEQAN_CONCEPT_IMPL(String<TValue, TSpec>, (StringConcept));
..include:seqan/basic.h
 */

# define SEQAN_CONCEPT_IMPL(model, implementedConcepts)                                                 \
    template <>                                                                                         \
    struct Implements<model>                                                                            \
    {                                                                                                   \
        typedef                                                                                         \
            SEQAN_PP_SEQ_FOR_EACH_I(SEQAN_CONCEPT_LIST_prefix,(model),implementedConcepts)              \
            SEQAN_PP_REPEAT(SEQAN_PP_SEQ_SIZE(implementedConcepts),SEQAN_CONCEPT_LIST_suffix,~) Type;   \
    }

    
// helper for the SEQAN_CONCEPT, above.
# define SEQAN_CONCEPT_typename(r, ignored, index, t) \
    SEQAN_PP_COMMA_IF(index) typename t

// helper for the SEQAN_CONCEPT, above.
# define SEQAN_CONCEPT_REFINE_superclass(r, params, index, t) \
    SEQAN_PP_COMMA_IF(index) t<SEQAN_PP_SEQ_ENUM(params)>
# define SEQAN_CONCEPT_LIST_prefix(r, params, index, t) \
    SEQAN_PP_COMMA_IF(index) TagList<t<SEQAN_PP_SEQ_ENUM(params)>
# define SEQAN_CONCEPT_LIST_suffix(z, n, text) > 

// ============================================================================
// Functions
// ============================================================================

/**
.Function.sameType:
..summary:Tests for equality of types.
..cat:Concepts
..signature:sameType(x, y)
..param.x: Object of a certain type.
..param.y: Object that must be of the same type.
..remarks:This function can be used to test for the correctness of function return types or the type of an expression.
..see:Macro.SEQAN_CONCEPT_USAGE
..include:seqan/basic.h
 */
template <typename T>
void sameType(T, T) { }


// ============================================================================
// Metafunctions
// ============================================================================

/**
.Metafunction.Is
..cat:Concepts
..summary:Returns whether a concept is fulfilled.
..signature:Is<TConcept>::Type
..signature:Is<TConcept>::VALUE
..param.TConcept:A concept that is specialized with type(s) that should be tested for fulfilling the concept.
..returns:@Tag.Logical Values.tag.True@/$true$ if $TConcept$ is a fulfilled concept, otherwise @Tag.Logical Values.tag.False@/$false$.
..remarks:
...text:The @Metafunction.Is@-metafunction can be used to test types for fulfilling a concept without causing compilation errors.
If @Tag.Logical Values.tag.True@/$true$ is returned, $TConcept$ must pass the concept test via @Macro.SEQAN_CONCEPT_ASSERT@.
It can be used to switch between different implementations depending on the concept of a type, or in combination with @Macro.SEQAN_FUNC_ENABLE_IF@ to make a function only visible to types of certain concepts.
..example.code:
Is<SequenceConcept<TSeq> >::Type
IfC<Is<ContainerConcept<TSeq> >::VALUE, T1, T2>::Type

std::cout << Is<IntegerConcept<int> >::VALUE << std::endl;     // 1
std::cout << Is<IntegerConcept<double> >::VALUE << std::endl;  // 0
..example.text:Define a hierarchy of concepts and two models $Alice$ and $Bob$ that implements some of them.
@Metafunction.Is@ determines which concepts are explicitly or implicitly fulfilled.
..example.code:
struct Alice {};
struct Bob {};

SEQAN_CONCEPT(ConceptA, (T)) {};
SEQAN_CONCEPT(ConceptB, (T)) {};
SEQAN_CONCEPT_REFINE(ConceptC, (T), (ConceptA)(ConceptB)) {};
SEQAN_CONCEPT_REFINE(ConceptD, (T), (ConceptC)) {};

SEQAN_CONCEPT_IMPL(Alice, (ConceptA)(ConceptB));
SEQAN_CONCEPT_IMPL(Bob, (ConceptC));

std::cout << Is< ConceptA<Alice> >::VALUE << std::endl; // 1
std::cout << Is< ConceptB<Alice> >::VALUE << std::endl; // 1
std::cout << Is< ConceptC<Alice> >::VALUE << std::endl; // 0
std::cout << Is< ConceptD<Alice> >::VALUE << std::endl; // 0

std::cout << Is< ConceptA<Bob> >::VALUE << std::endl;   // 1
std::cout << Is< ConceptB<Bob> >::VALUE << std::endl;   // 1
std::cout << Is< ConceptC<Bob> >::VALUE << std::endl;   // 1
std::cout << Is< ConceptD<Bob> >::VALUE << std::endl;   // 0
..include:seqan/basic.h
..see:Macro.SEQAN_FUNC_ENABLE_IF
..see:Macro.SEQAN_CONCEPT_ASSERT
*/

// test whether a concept is fulfilled (without concept checking)
template <typename T>
struct Implements: False {};

template <typename TModel>
struct Refines
{
    typedef void Type;
};


template <typename TConceptWithModel, typename TConceptWithModelList>
struct IsRecurse_: False {};

template <typename TConceptWithModel, typename TTail>
struct IsRecurse_<TConceptWithModel, TagList<TConceptWithModel, TTail> >: True {};

template <typename TConceptModel, typename THead, typename TTail>
struct IsRecurse_< TConceptModel, TagList<THead, TTail>  >:     
    Or< 
        IsRecurse_<TConceptModel, typename Refines<THead>::Type >,
        IsRecurse_<TConceptModel, TTail>
    > {};

template <typename TConceptWithModel>
struct Is;

template <template <typename> class TConcept, typename TModel>
struct Is< TConcept<TModel> >: 
    IsRecurse_<TConcept<TModel>, typename Implements<TModel>::Type> {};


}  // namespace seqan

#endif  // #ifndef CORE_INCLUDE_SEQAN_BASIC_CONCEPT_CHECKING_H_
