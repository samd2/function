// Boost.Function library

// Copyright (C) 2001 Doug Gregor (gregod@cs.rpi.edu)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.

// For more information, see http://www.boost.org

/* Note: this header is a header template and must NOT have multiple-inclusion
   protection. */

#include <cassert>
#include <algorithm>

namespace boost {
  namespace detail {
    namespace function {
#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      template<typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS>
      struct BOOST_FUNCTION_INVOKER_BASE
      {
        virtual ~BOOST_FUNCTION_INVOKER_BASE() {}
        virtual R call(BOOST_FUNCTION_PARMS) = 0;
        virtual R call(BOOST_FUNCTION_PARMS) const = 0;
        virtual BOOST_FUNCTION_INVOKER_BASE* clone() const = 0;
        virtual void destroy(BOOST_FUNCTION_INVOKER_BASE*) = 0;
      };
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS

      template<
        typename FunctionPtr,
#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        typename Allocator,
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
        >
      struct BOOST_FUNCTION_FUNCTION_INVOKER
#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        : public BOOST_FUNCTION_INVOKER_BASE<
                   R BOOST_FUNCTION_COMMA
                   BOOST_FUNCTION_TEMPLATE_ARGS
                 >
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      {
#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        explicit BOOST_FUNCTION_FUNCTION_INVOKER(FunctionPtr f) : 
          function_ptr(f) {}

        virtual R call(BOOST_FUNCTION_PARMS)
        {
          return function_ptr(BOOST_FUNCTION_ARGS);
        }

        virtual R call(BOOST_FUNCTION_PARMS) const
        {
          return function_ptr(BOOST_FUNCTION_ARGS);
        }

        virtual BOOST_FUNCTION_FUNCTION_INVOKER* clone() const
        {
#  ifdef BOOST_NO_STD_ALLOCATOR
          return new BOOST_FUNCTION_FUNCTION_INVOKER(function_ptr);
#  else
          typedef typename Allocator::
            template rebind<BOOST_FUNCTION_FUNCTION_INVOKER>::other 
            allocator_type;
          typedef typename allocator_type::pointer pointer_type;
          allocator_type allocator;

          pointer_type copy = allocator.allocate(1);
          allocator.construct(copy, *this);
          return static_cast<BOOST_FUNCTION_FUNCTION_INVOKER*>(copy);
#  endif // BOOST_NO_STD_ALLOCATOR
        }

        virtual void destroy(BOOST_FUNCTION_INVOKER_BASE<
                               R BOOST_FUNCTION_COMMA
                               BOOST_FUNCTION_TEMPLATE_ARGS
                             >* p)
        {
#  ifdef BOOST_NO_STD_ALLOCATOR
          delete p;
#  else
          BOOST_FUNCTION_FUNCTION_INVOKER* victim = 
            dynamic_cast<BOOST_FUNCTION_FUNCTION_INVOKER*>(p);

          typedef typename Allocator::
            template rebind<BOOST_FUNCTION_FUNCTION_INVOKER>::other 
            allocator_type;
          typedef typename allocator_type::pointer pointer_type;
          allocator_type allocator;

          allocator.destroy(victim);
          allocator.deallocate(victim, 1);
#  endif // BOOST_NO_STD_ALLOCATOR
        }

      private:
        FunctionPtr function_ptr;
#else
        static R invoke(any_pointer function_ptr,
                        bool BOOST_FUNCTION_COMMA
                        BOOST_FUNCTION_PARMS)
        {
          FunctionPtr f = reinterpret_cast<FunctionPtr>(function_ptr.func_ptr);
          return f(BOOST_FUNCTION_ARGS);
        }
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      };

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
      template<
        typename FunctionPtr,
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        typename Allocator,
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
        >
      struct BOOST_FUNCTION_VOID_FUNCTION_INVOKER
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        : public BOOST_FUNCTION_INVOKER_BASE<
                   unusable BOOST_FUNCTION_COMMA
                   BOOST_FUNCTION_TEMPLATE_ARGS
                 >
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      {
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        explicit BOOST_FUNCTION_VOID_FUNCTION_INVOKER(FunctionPtr f) : 
          function_ptr(f) {}

        virtual unusable call(BOOST_FUNCTION_PARMS)
        {
          function_ptr(BOOST_FUNCTION_ARGS);
          return unusable();
        }

        virtual unusable call(BOOST_FUNCTION_PARMS) const
        {
          function_ptr(BOOST_FUNCTION_ARGS);
          return unusable;
        }

        virtual BOOST_FUNCTION_VOID_FUNCTION_INVOKER* clone() const
        {
#    ifdef BOOST_NO_STD_ALLOCATOR
          return new BOOST_FUNCTION_VOID_FUNCTION_INVOKER(function_ptr);
#    else
          typedef typename Allocator::
            template rebind<BOOST_FUNCTION_VOID_FUNCTION_INVOKER>::other
            allocator_type;
          typedef typename allocator_type::pointer pointer_type;
          allocator_type allocator;

          pointer_type copy = allocator.allocate(1);
          allocator.construct(copy, *this);
          return static_cast<BOOST_FUNCTION_VOID_FUNCTION_INVOKER*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
        }

        virtual void destroy(BOOST_FUNCTION_INVOKER_BASE<
                               unusable BOOST_FUNCTION_COMMA
                               BOOST_FUNCTION_TEMPLATE_ARGS
                             >* p)
        {
#    ifdef BOOST_NO_STD_ALLOCATOR
          delete p;
#    else
          BOOST_FUNCTION_VOID_FUNCTION_INVOKER* victim =
            dynamic_cast<BOOST_FUNCTION_VOID_FUNCTION_INVOKER*>(p);

          typedef typename Allocator::
            template rebind<BOOST_FUNCTION_VOID_FUNCTION_INVOKER>::other
            allocator_type;
          typedef typename allocator_type::pointer pointer_type;
          allocator_type allocator;

          allocator.destroy(victim);
          allocator.deallocate(victim, 1);
#    endif // BOOST_NO_STD_ALLOCATOR
        }

      private:
        FunctionPtr function_ptr;
#  else
        static unusable invoke(any_pointer function_ptr,
                               bool BOOST_FUNCTION_COMMA
                               BOOST_FUNCTION_PARMS)

        {
          FunctionPtr f = reinterpret_cast<FunctionPtr>(function_ptr.func_ptr);
          f(BOOST_FUNCTION_ARGS);
          return unusable();
        }
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      };

#else
      template<
        typename FunctionPtr,
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        typename Allocator,
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
        >
      struct BOOST_FUNCTION_VOID_FUNCTION_INVOKER
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        : public BOOST_FUNCTION_INVOKER_BASE<
                   void BOOST_FUNCTION_COMMA
                   BOOST_FUNCTION_TEMPLATE_ARGS
                 >
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      {
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        explicit BOOST_FUNCTION_VOID_FUNCTION_INVOKER(FunctionPtr f) : 
          function_ptr(f) {}

        virtual void call(BOOST_FUNCTION_PARMS)
        {
          function_ptr(BOOST_FUNCTION_ARGS);
        }

        virtual void call(BOOST_FUNCTION_PARMS) const
        {
          function_ptr(BOOST_FUNCTION_ARGS);
        }

        virtual BOOST_FUNCTION_VOID_FUNCTION_INVOKER* clone() const
        {
#    ifdef BOOST_NO_STD_ALLOCATOR
          return new BOOST_FUNCTION_VOID_FUNCTION_INVOKER(function_ptr);
#    else
          typedef typename Allocator::
            template rebind<BOOST_FUNCTION_VOID_FUNCTION_INVOKER>::other
            allocator_type;
          typedef typename allocator_type::pointer pointer_type;
          allocator_type allocator;

          pointer_type copy = allocator.allocate(1);
          allocator.construct(copy, *this);
          return static_cast<BOOST_FUNCTION_VOID_FUNCTION_INVOKER   *>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
        }

        virtual void destroy(BOOST_FUNCTION_INVOKER_BASE<
                               void BOOST_FUNCTION_COMMA
                               BOOST_FUNCTION_TEMPLATE_ARGS
                             >* p)
        {
#    ifdef BOOST_NO_STD_ALLOCATOR
          delete p;
#    else
          BOOST_FUNCTION_VOID_FUNCTION_INVOKER* victim =
            dynamic_cast<BOOST_FUNCTION_VOID_FUNCTION_INVOKER*>(p);

          typedef typename Allocator::
            template rebind<BOOST_FUNCTION_VOID_FUNCTION_INVOKER>::other
            allocator_type;
          typedef typename allocator_type::pointer pointer_type;
          allocator_type allocator;

          allocator.destroy(victim);
          allocator.deallocate(victim, 1);
#    endif // BOOST_NO_STD_ALLOCATOR
        }

      private:
        FunctionPtr function_ptr;
#  else
        static void invoke(any_pointer function_ptr,
                           bool BOOST_FUNCTION_COMMA
                           BOOST_FUNCTION_PARMS)
        {
          FunctionPtr f = reinterpret_cast<FunctionPtr>(function_ptr.func_ptr);
          f(BOOST_FUNCTION_ARGS);
        }
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      };

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

      template<
        typename FunctionObj,
#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        typename Allocator,
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_FUNCTION_OBJ_INVOKER
#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        : public BOOST_FUNCTION_INVOKER_BASE<
                   R BOOST_FUNCTION_COMMA
                   BOOST_FUNCTION_TEMPLATE_ARGS
                 >
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      {
#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        explicit BOOST_FUNCTION_FUNCTION_OBJ_INVOKER(const FunctionObj& f) :
          function_obj(f) {}

        virtual R call(BOOST_FUNCTION_PARMS)
        {
          return function_obj(BOOST_FUNCTION_ARGS);
        }

        virtual R call(BOOST_FUNCTION_PARMS) const
        {
          return function_obj(BOOST_FUNCTION_ARGS);
        }

        virtual BOOST_FUNCTION_FUNCTION_OBJ_INVOKER* clone() const
        {
#ifdef BOOST_NO_STD_ALLOCATOR
          return new BOOST_FUNCTION_FUNCTION_OBJ_INVOKER(function_obj);
#else
          typedef typename Allocator::
            template rebind<BOOST_FUNCTION_FUNCTION_OBJ_INVOKER>::other
            allocator_type;
          typedef typename allocator_type::pointer pointer_type;
          allocator_type allocator;

          pointer_type copy = allocator.allocate(1);
          allocator.construct(copy, *this);
          return static_cast<BOOST_FUNCTION_FUNCTION_OBJ_INVOKER*>(copy);
#endif // BOOST_NO_STD_ALLOCATOR
        }

        virtual void destroy(BOOST_FUNCTION_INVOKER_BASE<
                               R BOOST_FUNCTION_COMMA
                               BOOST_FUNCTION_TEMPLATE_ARGS
                             >* p)
        {
#ifdef BOOST_NO_STD_ALLOCATOR
          delete p;
#else
          BOOST_FUNCTION_FUNCTION_OBJ_INVOKER* victim =
            dynamic_cast<BOOST_FUNCTION_FUNCTION_OBJ_INVOKER*>(p);

          typedef typename Allocator::
            template rebind<BOOST_FUNCTION_FUNCTION_OBJ_INVOKER>::other 
            allocator_type;
          typedef typename allocator_type::pointer pointer_type;
          allocator_type allocator;

          allocator.destroy(victim);
          allocator.deallocate(victim, 1);
#endif // BOOST_NO_STD_ALLOCATOR
        }

      private:
        FunctionObj function_obj;
#else
        static R invoke(any_pointer function_obj_ptr,
                        bool is_const BOOST_FUNCTION_COMMA
                        BOOST_FUNCTION_PARMS)

        {
          FunctionObj* f = static_cast<FunctionObj*>(function_obj_ptr.obj_ptr);
          if (is_const) {
            const FunctionObj* fc = f;
            return (*fc)(BOOST_FUNCTION_ARGS);
          }
          else {
            return (*f)(BOOST_FUNCTION_ARGS);
          }
        }
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      };

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
      template<
        typename FunctionObj,
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        typename Allocator,
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        : public BOOST_FUNCTION_INVOKER_BASE<
                   unusable BOOST_FUNCTION_COMMA
                   BOOST_FUNCTION_TEMPLATE_ARGS
                 >
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      {
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        explicit BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER(const FunctionObj& f)
          : function_obj(f) {}

        virtual unusable call(BOOST_FUNCTION_PARMS)
        {
          function_obj(BOOST_FUNCTION_ARGS);
          return unusable();
        }

        virtual unusable call(BOOST_FUNCTION_PARMS) const
        {
          function_obj(BOOST_FUNCTION_ARGS);
          return unusable();
        }

        virtual BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER* clone() const
        {
#ifdef BOOST_NO_STD_ALLOCATOR
          return new BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER(function_obj);
#else
          typedef typename Allocator::
            template rebind<BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER>::other
            allocator_type;
          typedef typename allocator_type::pointer pointer_type;
          allocator_type allocator;

          pointer_type copy = allocator.allocate(1);
          allocator.construct(copy, *this);
          return static_cast<BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER*>(copy);
#endif // BOOST_NO_STD_ALLOCATOR
        }

        virtual void destroy(BOOST_FUNCTION_INVOKER_BASE<
                               unusable BOOST_FUNCTION_COMMA
                               BOOST_FUNCTION_TEMPLATE_ARGS
                             >* p)
        {
#ifdef BOOST_NO_STD_ALLOCATOR
          delete p;
#else
          BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER* victim =
            dynamic_cast<BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER*>(p);

          typedef typename Allocator::
            template rebind<BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER>::other
            allocator_type;
          typedef typename allocator_type::pointer pointer_type;
          allocator_type allocator;

          allocator.destroy(victim);
          allocator.deallocate(victim, 1);
#endif // BOOST_NO_STD_ALLOCATOR
        }

      private:
        FunctionObj function_obj;
#  else
        static unusable invoke(any_pointer function_obj_ptr,
                               bool is_const BOOST_FUNCTION_COMMA
                               BOOST_FUNCTION_PARMS)

        {
          FunctionObj* f = static_cast<FunctionObj*>(function_obj_ptr.obj_ptr);
          if (is_const) {
            const FunctionObj* fc = f;
            (*fc)(BOOST_FUNCTION_ARGS);
          }
          else {
            (*f)(BOOST_FUNCTION_ARGS);
          }

          return unusable();
        }
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      };
#else
      template<
        typename FunctionObj,
#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        typename Allocator,
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
        >
      struct BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        : public BOOST_FUNCTION_INVOKER_BASE<
                   void BOOST_FUNCTION_COMMA
                   BOOST_FUNCTION_TEMPLATE_ARGS
                 >
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      {
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
        explicit BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER(const FunctionObj& f)
          : function_obj(f) {}

        virtual void call(BOOST_FUNCTION_PARMS)
        {
          function_obj(BOOST_FUNCTION_ARGS);
        }

        virtual void call(BOOST_FUNCTION_PARMS) const
        {
          function_obj(BOOST_FUNCTION_ARGS);
        }

        virtual BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER * clone() const
        {
#    ifdef BOOST_NO_STD_ALLOCATOR
          return new BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER   (function_obj);
#    else
          typedef typename Allocator::
            template rebind<BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER>::other
            allocator_type;
          typedef typename allocator_type::pointer pointer_type;
          allocator_type allocator;

          pointer_type copy = allocator.allocate(1);
          allocator.construct(copy, *this);
          return static_cast<BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
        }

        virtual void destroy(BOOST_FUNCTION_INVOKER_BASE<
                               void BOOST_FUNCTION_COMMA
                               BOOST_FUNCTION_TEMPLATE_ARGS
                             >* p)
        {
#ifdef BOOST_NO_STD_ALLOCATOR
          delete p;
#else
          BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER* victim =
            dynamic_cast<BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER*>(p);

          typedef typename Allocator::
            template rebind<BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER>::other
            allocator_type;
          typedef typename allocator_type::pointer pointer_type;
          allocator_type allocator;

          allocator.destroy(victim);
          allocator.deallocate(victim, 1);
#endif // BOOST_NO_STD_ALLOCATOR
        }

      private:
        FunctionObj function_obj;
#  else
        static void
        invoke(any_pointer function_obj_ptr,
               bool is_const BOOST_FUNCTION_COMMA
               BOOST_FUNCTION_PARMS)

        {
          FunctionObj* f = static_cast<FunctionObj*>(function_obj_ptr.obj_ptr);
          if (is_const) {
            const FunctionObj* fc = f;
            (*fc)(BOOST_FUNCTION_ARGS);
          }
          else {
            (*f)(BOOST_FUNCTION_ARGS);
          }
        }
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      };
#endif

      template<
        typename FunctionPtr,
        typename Allocator,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_GET_FUNCTION_INVOKER
      {
        typedef typename IF<(is_void<R>::value),
                            BOOST_FUNCTION_VOID_FUNCTION_INVOKER<
                            FunctionPtr,
#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
                            Allocator,
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >,
                          BOOST_FUNCTION_FUNCTION_INVOKER<
                            FunctionPtr,
#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
                            Allocator,
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >
                       >::type type;
      };

      template<
        typename FunctionObj,
        typename Allocator,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
       >
      struct BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER
      {
        typedef typename IF<(is_void<R>::value),
                            BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER<
                            FunctionObj,
#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
                            Allocator,
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >,
                          BOOST_FUNCTION_FUNCTION_OBJ_INVOKER<
                            FunctionObj,
#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
                            Allocator,
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >
                       >::type type;
      };
    } // end namespace function
  } // end namespace detail

  template<
    typename R BOOST_FUNCTION_COMMA
    BOOST_FUNCTION_TEMPLATE_PARMS,
    typename Policy    = empty_function_policy,
    typename Mixin     = empty_function_mixin,
    typename Allocator = std::allocator<function_base>
  >
  class BOOST_FUNCTION_FUNCTION : public function_base, public Mixin
  {
  public:
    BOOST_STATIC_CONSTANT(int, args = BOOST_FUNCTION_NUM_ARGS);

    typedef typename detail::function::function_return_type<R>::type 
      result_type;
    typedef Policy policy_type;
    typedef Mixin mixin_type;
    typedef Allocator allocator_type;

#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
  private:
    typedef detail::function::BOOST_FUNCTION_INVOKER_BASE<
              result_type BOOST_FUNCTION_COMMA
              BOOST_FUNCTION_TEMPLATE_ARGS
            >
      impl_type;

  public:
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS


#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
    // Construct without a target
    BOOST_FUNCTION_FUNCTION() : function_base() {}
#else
    // Construct without a target
    BOOST_FUNCTION_FUNCTION() : function_base()
# ifndef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
                     , invoker(0)
# endif
    {
    }

#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS

#ifdef BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING
    // Clone the functor target f and make the clone the target
    template<typename Functor>
    BOOST_FUNCTION_FUNCTION(Functor BOOST_MSVC_INCLUDE(const &) f) :
      function_base()
# ifndef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      , invoker(0)
# endif
    {
      if (!detail::function::has_empty_target(f))
        assign_to(f);
    }
#else
   template<typename Functor>
   BOOST_FUNCTION_FUNCTION(const Functor& f) : function_base()
#  ifndef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
                                    , invoker(0)
#  endif
   {
     if (!detail::function::has_empty_target(&f)) {
       typedef 
         typename detail::function::BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER<
                    Functor,
                    Allocator,
                    R BOOST_FUNCTION_COMMA
                    BOOST_FUNCTION_TEMPLATE_ARGS
                  >::type
         invoker_type;

#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
#    ifdef BOOST_NO_STD_ALLOCATOR
       impl_type* i = new invoker_type(f);
#    else
       typedef typename Allocator::template rebind<invoker_type>::other
         allocator_type;
       typedef typename allocator_type::pointer pointer_type;
       allocator_type allocator;

       pointer_type copy = allocator.allocate(1);
       new (copy) invoker_type(f);
       impl_type* i = static_cast<impl_type*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
       impl = static_cast<void*>(i);
#  else
       invoker = &invoker_type::invoke;
       manager = &detail::function::functor_manager<Functor, Allocator>::manage;
       functor = manager(detail::function::any_pointer(const_cast<Functor*>(&f)),
                         detail::function::clone_functor);
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
     }
   }

   template<typename OtherR BOOST_FUNCTION_COMMA 
            BOOST_FUNCTION_OTHER_TEMPLATE_PARMS>
   BOOST_FUNCTION_FUNCTION(OtherR (*f)(
                            BOOST_FUNCTION_OTHER_TEMPLATE_ARGS
                            )) : function_base()
# ifndef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
                                 , invoker(0)
# endif
   {
     if (f) {
       typedef OtherR (*FunctionPtr)(BOOST_FUNCTION_OTHER_TEMPLATE_ARGS);
       typedef typename detail::function::BOOST_FUNCTION_GET_FUNCTION_INVOKER<
                          FunctionPtr,
                          Allocator,
                          R BOOST_FUNCTION_COMMA
                          BOOST_FUNCTION_TEMPLATE_ARGS
                        >::type
         invoker_type;

#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
#    ifdef BOOST_NO_STD_ALLOCATOR
       impl_type* i = new invoker_type(f);
#    else
       typedef typename Allocator::template rebind<invoker_type>::other
         allocator_type;
       typedef typename allocator_type::pointer pointer_type;
       allocator_type allocator;

       pointer_type copy = allocator.allocate(1);
       new (copy) invoker_type(f);
       impl_type* i = static_cast<impl_type*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
       impl = static_cast<void*>(i);
#  else
       invoker = &invoker_type::invoke;
       manager = &detail::function::functor_manager<FunctionPtr, Allocator>
                   ::manage;
       functor = manager(detail::function::any_pointer(reinterpret_cast<void (*)()>(f)),
                         detail::function::clone_functor);
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
     }
   }
#endif // BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING

    // Clone the input BOOST_FUNCTION_FUNCTION's target
    BOOST_FUNCTION_FUNCTION(const BOOST_FUNCTION_FUNCTION& f): function_base()
# ifndef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
                                         , invoker(0)
# endif
    {
      if (!f.empty()) {
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
       impl_type* other_impl = reinterpret_cast<impl_type*>(f.impl);
       impl = static_cast<void*>(other_impl->clone());
#  else
        invoker = f.invoker;
        manager = f.manager;
        functor = f.manager(f.functor, detail::function::clone_functor);
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      }
    }

    // Destroy the target, if there is one
    ~BOOST_FUNCTION_FUNCTION() { clear(); }

    void swap(BOOST_FUNCTION_FUNCTION& other)
    {
#ifndef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      std::swap(manager, other.manager);
      std::swap(functor, other.functor);
      std::swap(invoker, other.invoker);
#else
      std::swap(impl, other.impl);
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
    }

    // Invoke the target
    result_type operator()(BOOST_FUNCTION_PARMS)
    {
      assert(!this->empty());

      policy_type policy;
      policy.precall(this);

#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      impl_type* i = reinterpret_cast<impl_type*>(impl);
      result_type result = i->call(BOOST_FUNCTION_ARGS);
#else
      result_type result = invoker(functor,
                                   false BOOST_FUNCTION_COMMA
                                   BOOST_FUNCTION_ARGS);
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS

      policy.postcall(this);
      return result;
    }

    result_type operator()(BOOST_FUNCTION_PARMS) const
    {
      assert(!this->empty());

      policy_type policy;
      policy.precall(this);

#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      const impl_type* i = reinterpret_cast<const impl_type*>(impl);
      result_type result = i->call(BOOST_FUNCTION_ARGS);
#else
      result_type result = invoker(functor,
                                   true BOOST_FUNCTION_COMMA
                                   BOOST_FUNCTION_ARGS);
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS

      policy.postcall(this);
      return result;
    }

    // Clear out a target, if there is one
    void clear()
    {
#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      if (impl) {
        impl_type* i = reinterpret_cast<impl_type*>(impl);
        i->destroy(i);
        impl = 0;
      }
#else
      if (manager)
        functor = manager(functor, detail::function::destroy_functor);

      manager = 0;
      invoker = 0;
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
    }

#ifdef BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING
    // Assignment from any functor
    template<typename Functor>
    BOOST_FUNCTION_FUNCTION& operator=(Functor BOOST_MSVC_INCLUDE(const &) f)
    {
      clear();

      if (!detail::function::has_empty_target(f)) {
        // MSVC 6 eats the ';', so add another ';'. Don't ask me why.
        assign_to(f);;
      }

      return *this;
    }
#else
   template<typename Functor>
   BOOST_FUNCTION_FUNCTION& operator=(const Functor& f)
   {
     clear();

     if (!detail::function::has_empty_target(&f)) {
       typedef 
         typename detail::function::BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER<
                    Functor,
                    Allocator,
                    R BOOST_FUNCTION_COMMA
                    BOOST_FUNCTION_TEMPLATE_ARGS
                  >::type
         invoker_type;

#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
#    ifdef BOOST_NO_STD_ALLOCATOR
       impl_type* i = new invoker_type(f);
#    else
       typedef typename Allocator::template rebind<invoker_type>::other
         allocator_type;
       typedef typename allocator_type::pointer pointer_type;
       allocator_type allocator;

       pointer_type copy = allocator.allocate(1);
       new (copy) invoker_type(f);
       impl_type* i = static_cast<impl_type*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
       impl = static_cast<void*>(i);
#  else
       invoker = &invoker_type::invoke;
       manager = &detail::function::functor_manager<Functor, Allocator>::manage;
       functor = manager(detail::function::any_pointer(const_cast<Functor*>(&f)),
                         detail::function::clone_functor);
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
     }

     return *this;
   }

   template<typename OtherR BOOST_FUNCTION_COMMA
            BOOST_FUNCTION_OTHER_TEMPLATE_PARMS>
   BOOST_FUNCTION_FUNCTION& 
   operator=(OtherR (*f)(BOOST_FUNCTION_OTHER_TEMPLATE_ARGS))
   {
     clear();

     if (f) {
       typedef OtherR (*FunctionPtr)(BOOST_FUNCTION_OTHER_TEMPLATE_ARGS);
       typedef typename detail::function::BOOST_FUNCTION_GET_FUNCTION_INVOKER<
                                  FunctionPtr,
                                  Allocator,
                                  R BOOST_FUNCTION_COMMA
                                  BOOST_FUNCTION_TEMPLATE_ARGS
                                >::type
         invoker_type;

#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
#    ifdef BOOST_NO_STD_ALLOCATOR
       impl_type* i = new invoker_type(f);
#    else
       typedef typename Allocator::template rebind<invoker_type>::other
         allocator_type;
       typedef typename allocator_type::pointer pointer_type;
       allocator_type allocator;

       pointer_type copy = allocator.allocate(1);
       new (copy) invoker_type(f);
       impl_type* i = static_cast<impl_type*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
       impl = static_cast<void*>(i);
#  else
       invoker = &invoker_type::invoke;
       manager = &detail::function::functor_manager<FunctionPtr, Allocator>::manage;
       functor = manager(detail::function::any_pointer(reinterpret_cast<void (*)()>(f)),
                         detail::function::clone_functor);
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
     }

     return *this;
   }
#endif // BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING

#ifdef BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING
    // Assignment from any functor
    template<typename Functor>
    void set(Functor BOOST_MSVC_INCLUDE(const &) f)
    {
      clear();

      if (!detail::function::has_empty_target(f)) {
        // MSVC 6 eats the ';', so add another ';'. Don't ask me why.
        assign_to(f);;
      }
    }
#else
   template<typename Functor>
   void set(const Functor& f)
   {
     clear();

     if (!detail::function::has_empty_target(&f)) {
       typedef 
         typename detail::function::BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER<
                    Functor,
                    Allocator,
                    R BOOST_FUNCTION_COMMA
                    BOOST_FUNCTION_TEMPLATE_ARGS
                  >::type
         invoker_type;

#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
#    ifdef BOOST_NO_STD_ALLOCATOR
       impl_type* i = new invoker_type(f);
#    else
       typedef typename Allocator::template rebind<invoker_type>::other
         allocator_type;
       typedef typename allocator_type::pointer pointer_type;
       allocator_type allocator;

       pointer_type copy = allocator.allocate(1);
       new (copy) invoker_type(f);
       impl_type* i = static_cast<impl_type*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
       impl = static_cast<void*>(i);
#  else
       invoker = &invoker_type::invoke;
       manager = &detail::function::functor_manager<Functor, Allocator>::manage;
       functor = manager(detail::function::any_pointer(const_cast<Functor*>(&f)),
                         detail::function::clone_functor);
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
     }
   }

   template<typename OtherR BOOST_FUNCTION_COMMA
            BOOST_FUNCTION_OTHER_TEMPLATE_PARMS>
   void set(OtherR (*f)(BOOST_FUNCTION_OTHER_TEMPLATE_ARGS))
   {
     clear();

     if (f) {
       typedef OtherR (*FunctionPtr)(BOOST_FUNCTION_OTHER_TEMPLATE_ARGS);
       typedef typename detail::function::BOOST_FUNCTION_GET_FUNCTION_INVOKER<
                                  FunctionPtr,
                                  Allocator,
                                  R BOOST_FUNCTION_COMMA
                                  BOOST_FUNCTION_TEMPLATE_ARGS
                                >::type
         invoker_type;

#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
#    ifdef BOOST_NO_STD_ALLOCATOR
       impl_type* i = new invoker_type(f);
#    else
       typedef typename Allocator::template rebind<invoker_type>::other
         allocator_type;
       typedef typename allocator_type::pointer pointer_type;
       allocator_type allocator;

       pointer_type copy = allocator.allocate(1);
       new (copy) invoker_type(f);
       impl_type* i = static_cast<impl_type*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
       impl = static_cast<void*>(i);
#  else
       invoker = &invoker_type::invoke;
       manager = &detail::function::functor_manager<FunctionPtr, Allocator>
                    ::manage;
       functor = manager(detail::function::any_pointer(reinterpret_cast<void (*)()>(f)),
                         detail::function::clone_functor);
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
     }
   }
#endif // BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING

    // Assignment from another BOOST_FUNCTION_FUNCTION
    BOOST_FUNCTION_FUNCTION& operator=(const BOOST_FUNCTION_FUNCTION& f)
    {
      clear();

      if (!f.empty()) {
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
       const impl_type* other_impl =
         reinterpret_cast<const impl_type*>(f.impl);
       impl = static_cast<void*>(other_impl->clone());
#  else
        invoker = f.invoker;
        manager = f.manager;
        functor = f.manager?
          f.manager(f.functor, detail::function::clone_functor) : f.functor;
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      }
      return *this;
    }

    // Assignment from another BOOST_FUNCTION_FUNCTION
    void set(const BOOST_FUNCTION_FUNCTION& f)
    {
      clear();

      if (!f.empty()) {
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
       const impl_type* other_impl =
         reinterpret_cast<const impl_type*>(f.impl);
       impl = static_cast<void*>(other_impl->clone());
#  else
        invoker = f.invoker;
        manager = f.manager;
        functor = f.manager?
          f.manager(f.functor, detail::function::clone_functor) : f.functor;
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      }
    }

  protected:
#if defined BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING || defined BOOST_NO_DEPENDENT_BASE_LOOKUP

#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
   // Clone a function pointer
   template<typename FunctionPtr>
   void* clone_functor(FunctionPtr f, detail::function::function_ptr_tag)
   {
     typedef typename detail::function::BOOST_FUNCTION_GET_FUNCTION_INVOKER<
                        FunctionPtr,
                        Allocator,
                        R BOOST_FUNCTION_COMMA
                        BOOST_FUNCTION_TEMPLATE_ARGS
                      >::type
         invoker_type;

#    ifdef BOOST_NO_STD_ALLOCATOR
     impl_type* i = new invoker_type(f);
#    else
     typedef typename Allocator::template rebind<invoker_type>::other
       allocator_type;
     typedef typename allocator_type::pointer pointer_type;
     allocator_type allocator;

     pointer_type copy = allocator.allocate(1);
     new (copy) invoker_type(f);
     impl_type* i = static_cast<impl_type*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
     return static_cast<void*>(i);
   }

   // Clone a function object
   template<typename FunctionObj>
   void* clone_functor(FunctionObj& f, detail::function::function_obj_tag)
   {
     typedef typename detail::function::BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER<
                        FunctionObj,
                        Allocator,
                        R BOOST_FUNCTION_COMMA
                        BOOST_FUNCTION_TEMPLATE_ARGS
                      >::type
         invoker_type;

#    ifdef BOOST_NO_STD_ALLOCATOR
     impl_type* i = new invoker_type(f);
#    else
     typedef typename Allocator::template rebind<invoker_type>::other
       allocator_type;
     typedef typename allocator_type::pointer pointer_type;
     allocator_type allocator;

     pointer_type copy = allocator.allocate(1);
     new (copy) invoker_type(f);
     impl_type* i = static_cast<impl_type*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
     return static_cast<void*>(i);
   }
#  else
    // Clone a function pointer
    template<typename FunctionPtr>
    detail::function::any_pointer 
    clone_functor(FunctionPtr f, detail::function::function_ptr_tag)
    {
      detail::function::any_pointer a(reinterpret_cast<void (*)()>(f));
      return manager(a, detail::function::clone_functor);
    }

    // Clone a functor
    template<typename FunctionObj>
    detail::function::any_pointer 
    clone_functor(FunctionObj& f, detail::function::function_obj_tag)
    {
      detail::function::any_pointer a(static_cast<void*>(&f));
      return manager(a, detail::function::clone_functor);
    }
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS

    // Assign this BOOST_FUNCTION_FUNCTION to the given functor
    template<typename Functor>
    void assign_to(Functor f)
    {
      // Do we have a functor or a function pointer?
      typedef typename detail::function::IF<
                         (is_pointer<Functor>::value),
                         detail::function::function_ptr_tag,
                         detail::function::function_obj_tag>::RET
        target_type;

#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      impl = clone_functor(f, target_type());
#  else
      typedef typename detail::function::IF<(is_pointer<Functor>::value),
                         detail::function::BOOST_FUNCTION_GET_FUNCTION_INVOKER<
                           Functor,
                           Allocator,
                           R BOOST_FUNCTION_COMMA
                           BOOST_FUNCTION_TEMPLATE_ARGS
                         >,
                         detail::function::BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER<
                           Functor,
                           Allocator,
                           R BOOST_FUNCTION_COMMA
                           BOOST_FUNCTION_TEMPLATE_ARGS
                         >
                       >::type
        invoker_selector;

      typedef typename invoker_selector::type invoker_type;

      // Setup the new functor
      invoker = &invoker_type::invoke;
      manager = &detail::function::functor_manager<Functor, Allocator>::manage;
      functor = clone_functor(f, target_type());
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
    }
#endif // BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING

#ifndef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
    typedef result_type (*invoker_type)(detail::function::any_pointer,
                                        bool BOOST_FUNCTION_COMMA
                                        BOOST_FUNCTION_TEMPLATE_ARGS
                                       );

    invoker_type invoker;
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
  };

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
  template<
    BOOST_FUNCTION_TEMPLATE_PARMS BOOST_FUNCTION_COMMA
    typename Policy,
    typename Mixin,
    typename Allocator
  >
  class BOOST_FUNCTION_FUNCTION<
          void BOOST_FUNCTION_COMMA
          BOOST_FUNCTION_TEMPLATE_ARGS,
          Policy,
          Mixin,
          Allocator> : public function_base, public Mixin
  {
  public:
    BOOST_STATIC_CONSTANT(int, args = BOOST_FUNCTION_NUM_ARGS);

    typedef void result_type;
    typedef void R;
    typedef Policy policy_type;
    typedef Mixin mixin_type;
    typedef Allocator allocator_type;

#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
  private:
    typedef detail::function::BOOST_FUNCTION_INVOKER_BASE<
              result_type BOOST_FUNCTION_COMMA
              BOOST_FUNCTION_TEMPLATE_ARGS
            >
      impl_type;

  public:
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS


#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
    // Construct without a target
    BOOST_FUNCTION_FUNCTION() : function_base() {}
#else
    // Construct without a target
    BOOST_FUNCTION_FUNCTION() : function_base()
# ifndef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
                     , invoker(0)
# endif
    {
    }

#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS

#ifdef BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING
    // Clone the functor target f and make the clone the target
    template<typename Functor>
    BOOST_FUNCTION_FUNCTION(Functor BOOST_MSVC_INCLUDE(const &) f) :
      function_base()
# ifndef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      , invoker(0)
# endif
    {
      if (!detail::function::has_empty_target(f))
        assign_to(f);
    }
#else
   template<typename Functor>
   BOOST_FUNCTION_FUNCTION(const Functor& f) : function_base()
#  ifndef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
                                    , invoker(0)
#  endif
   {
     if (!detail::function::has_empty_target(&f)) {
       typedef typename detail::function::BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER<
                                  Functor,
                                  Allocator,
                                  R BOOST_FUNCTION_COMMA
                                  BOOST_FUNCTION_TEMPLATE_ARGS
                                >::type
         invoker_type;

#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
#    ifdef BOOST_NO_STD_ALLOCATOR
       impl_type* i = new invoker_type(f);
#    else
       typedef typename Allocator::template rebind<invoker_type>::other
         allocator_type;
       typedef typename allocator_type::pointer pointer_type;
       allocator_type allocator;

       pointer_type copy = allocator.allocate(1);
       new (copy) invoker_type(f);
       impl_type* i = static_cast<impl_type*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
       impl = static_cast<void*>(i);
#  else
       invoker = &invoker_type::invoke;
       manager = &detail::function::functor_manager<Functor, Allocator>::manage;
       functor = manager(detail::function::any_pointer(const_cast<Functor*>(&f)),
                         detail::function::clone_functor);
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
     }
   }

   template<typename OtherR BOOST_FUNCTION_COMMA
            BOOST_FUNCTION_OTHER_TEMPLATE_PARMS>
   BOOST_FUNCTION_FUNCTION(OtherR (*f)(
                            BOOST_FUNCTION_OTHER_TEMPLATE_ARGS
                            )) : function_base()
# ifndef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
                                 , invoker(0)
# endif
   {
     if (f) {
       typedef OtherR (*FunctionPtr)(BOOST_FUNCTION_OTHER_TEMPLATE_ARGS);
       typedef typename detail::function::BOOST_FUNCTION_GET_FUNCTION_INVOKER<
                                  FunctionPtr,
                                  Allocator,
                                  R BOOST_FUNCTION_COMMA
                                  BOOST_FUNCTION_TEMPLATE_ARGS
                                >::type
         invoker_type;

#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
#    ifdef BOOST_NO_STD_ALLOCATOR
       impl_type* i = new invoker_type(f);
#    else
       typedef typename Allocator::template rebind<invoker_type>::other
         allocator_type;
       typedef typename allocator_type::pointer pointer_type;
       allocator_type allocator;

       pointer_type copy = allocator.allocate(1);
       new (copy) invoker_type(f);
       impl_type* i = static_cast<impl_type*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
       impl = static_cast<void*>(i);
#  else
       invoker = &invoker_type::invoke;
       manager = &detail::function::functor_manager<FunctionPtr, Allocator>::manage;
       functor = manager(detail::function::any_pointer(reinterpret_cast<void (*)()>(f)),
                         detail::function::clone_functor);
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
     }
   }
#endif // BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING

    // Clone the input BOOST_FUNCTION_FUNCTION's target
    BOOST_FUNCTION_FUNCTION(const BOOST_FUNCTION_FUNCTION& f): function_base()
# ifndef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
                                         , invoker(0)
# endif
    {
      if (!f.empty()) {
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
       impl_type* other_impl = reinterpret_cast<impl_type*>(f.impl);
       impl = static_cast<void*>(other_impl->clone());
#  else
        invoker = f.invoker;
        manager = f.manager;
        functor = f.manager(f.functor, detail::function::clone_functor);
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      }
    }

    // Destroy the target, if there is one
    ~BOOST_FUNCTION_FUNCTION() { clear(); }

    void swap(BOOST_FUNCTION_FUNCTION& other)
    {
#  ifndef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      std::swap(manager, other.manager);
      std::swap(functor, other.functor);
      std::swap(invoker, other.invoker);
#  else
      std::swap(impl, other.impl);
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
    }

    // Invoke the target
    result_type operator()(BOOST_FUNCTION_PARMS)
    {
      assert(!this->empty());
      policy_type policy;
      policy.precall(this);

#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      impl_type* i = reinterpret_cast<impl_type*>(impl);
      i->call(BOOST_FUNCTION_ARGS);
#else
      invoker(functor, false BOOST_FUNCTION_COMMA BOOST_FUNCTION_ARGS);
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS

      policy.postcall(this);
    }

    result_type operator()(BOOST_FUNCTION_PARMS) const
    {
      assert(!this->empty());

      policy_type policy;
      policy.precall(this);

#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      const impl_type* i = reinterpret_cast<const impl_type*>(impl);
      i->call(BOOST_FUNCTION_ARGS);
#else
      invoker(functor, true BOOST_FUNCTION_COMMA BOOST_FUNCTION_ARGS);
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS

      policy.postcall(this);
    }

    // Clear out a target, if there is one
    void clear()
    {
#ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      if (impl) {
        impl_type* i = reinterpret_cast<impl_type*>(impl);
        i->destroy(i);
        impl = 0;
      }
#else
      if (manager)
        functor = manager(functor, detail::function::destroy_functor);

      manager = 0;
      invoker = 0;
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
    }

#ifdef BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING
    // Assignment from any functor
    template<typename Functor>
    BOOST_FUNCTION_FUNCTION& operator=(Functor BOOST_MSVC_INCLUDE(const &) f)
    {
      clear();

      if (!detail::function::has_empty_target(f)) {
        // MSVC 6 eats the ';', so add another ';'. Don't ask me why.
        assign_to(f);;
      }

      return *this;
    }
#else
   template<typename Functor>
   BOOST_FUNCTION_FUNCTION& operator=(const Functor& f)
   {
     clear();

     if (!detail::function::has_empty_target(&f)) {
       typedef typename detail::function::BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER<
                                  Functor,
                                  Allocator,
                                  R BOOST_FUNCTION_COMMA
                                  BOOST_FUNCTION_TEMPLATE_ARGS
                                >::type
         invoker_type;

#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
#    ifdef BOOST_NO_STD_ALLOCATOR
       impl_type* i = new invoker_type(f);
#    else
       typedef typename Allocator::template rebind<invoker_type>::other
         allocator_type;
       typedef typename allocator_type::pointer pointer_type;
       allocator_type allocator;

       pointer_type copy = allocator.allocate(1);
       new (copy) invoker_type(f);
       impl_type* i = static_cast<impl_type*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
       impl = static_cast<void*>(i);
#  else
       invoker = &invoker_type::invoke;
       manager = &detail::function::functor_manager<Functor, Allocator>::manage;
       functor = manager(detail::function::any_pointer(const_cast<Functor*>(&f)),
                         detail::function::clone_functor);
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
     }

     return *this;
   }

   template<typename OtherR BOOST_FUNCTION_COMMA
            BOOST_FUNCTION_OTHER_TEMPLATE_PARMS>
   BOOST_FUNCTION_FUNCTION& 
   operator=(OtherR (*f)(BOOST_FUNCTION_OTHER_TEMPLATE_ARGS))
   {
     clear();

     if (f) {
       typedef OtherR (*FunctionPtr)(BOOST_FUNCTION_OTHER_TEMPLATE_ARGS);
       typedef typename detail::function::BOOST_FUNCTION_GET_FUNCTION_INVOKER<
                                  FunctionPtr,
                                  Allocator,
                                  R BOOST_FUNCTION_COMMA
                                  BOOST_FUNCTION_TEMPLATE_ARGS
                                >::type
         invoker_type;

#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
#    ifdef BOOST_NO_STD_ALLOCATOR
       impl_type* i = new invoker_type(f);
#    else
       typedef typename Allocator::template rebind<invoker_type>::other
         allocator_type;
       typedef typename allocator_type::pointer pointer_type;
       allocator_type allocator;

       pointer_type copy = allocator.allocate(1);
       new (copy) invoker_type(f);
       impl_type* i = static_cast<impl_type*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
       impl = static_cast<void*>(i);
#  else
       invoker = &invoker_type::invoke;
       manager = &detail::function::functor_manager<FunctionPtr, Allocator>::manage;
       functor = manager(detail::function::any_pointer(reinterpret_cast<void (*)()>(f)),
                         detail::function::clone_functor);
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
     }

     return *this;
   }
#endif // BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING

#ifdef BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING
    // Assignment from any functor
    template<typename Functor>
    void set(Functor BOOST_MSVC_INCLUDE(const &) f)
    {
      clear();

      if (!detail::function::has_empty_target(f)) {
        // MSVC 6 eats the ';', so add another ';'. Don't ask me why.
        assign_to(f);;
      }
    }
#else
   template<typename Functor>
   void set(const Functor& f)
   {
     clear();

     if (!detail::function::has_empty_target(&f)) {
       typedef typename detail::function::BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER<
                                  Functor,
                                  Allocator,
                                  R BOOST_FUNCTION_COMMA
                                  BOOST_FUNCTION_TEMPLATE_ARGS
                                >::type
         invoker_type;

#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
#    ifdef BOOST_NO_STD_ALLOCATOR
       impl_type* i = new invoker_type(f);
#    else
       typedef typename Allocator::template rebind<invoker_type>::other
         allocator_type;
       typedef typename allocator_type::pointer pointer_type;
       allocator_type allocator;

       pointer_type copy = allocator.allocate(1);
       new (copy) invoker_type(f);
       impl_type* i = static_cast<impl_type*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
       impl = static_cast<void*>(i);
#  else
       invoker = &invoker_type::invoke;
       manager = &detail::function::functor_manager<Functor, Allocator>::manage;
       functor = manager(detail::function::any_pointer(const_cast<Functor*>(&f)),
                         detail::function::clone_functor);
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
     }
   }

   template<typename OtherR BOOST_FUNCTION_COMMA
            BOOST_FUNCTION_OTHER_TEMPLATE_PARMS>
   void set(OtherR (*f)(BOOST_FUNCTION_OTHER_TEMPLATE_ARGS))
   {
     clear();

     if (f) {
       typedef OtherR (*FunctionPtr)(BOOST_FUNCTION_OTHER_TEMPLATE_ARGS);
       typedef typename detail::function::BOOST_FUNCTION_GET_FUNCTION_INVOKER<
                                  FunctionPtr,
                                  Allocator,
                                  R BOOST_FUNCTION_COMMA
                                  BOOST_FUNCTION_TEMPLATE_ARGS
                                >::type
         invoker_type;

#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
#    ifdef BOOST_NO_STD_ALLOCATOR
       impl_type* i = new invoker_type(f);
#    else
       typedef typename Allocator::template rebind<invoker_type>::other
         allocator_type;
       typedef typename allocator_type::pointer pointer_type;
       allocator_type allocator;

       pointer_type copy = allocator.allocate(1);
       new (copy) invoker_type(f);
       impl_type* i = static_cast<impl_type*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
       impl = static_cast<void*>(i);
#  else
       invoker = &invoker_type::invoke;
       manager = &detail::function::functor_manager<FunctionPtr, Allocator>::manage;
       functor = manager(detail::function::any_pointer(reinterpret_cast<void (*)()>(f)),
                         detail::function::clone_functor);
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
     }
   }
#endif // BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING

    // Assignment from another BOOST_FUNCTION_FUNCTION
    BOOST_FUNCTION_FUNCTION& operator=(const BOOST_FUNCTION_FUNCTION& f)
    {
      clear();

      if (!f.empty()) {
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
       const impl_type* other_impl =
         reinterpret_cast<const impl_type*>(f.impl);
       impl = static_cast<void*>(other_impl->clone());
#  else
        invoker = f.invoker;
        manager = f.manager;
        functor = f.manager?
                    f.manager(f.functor, detail::function::clone_functor) : f.functor;
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      }
      return *this;
    }

    // Assignment from another BOOST_FUNCTION_FUNCTION
    void set(const BOOST_FUNCTION_FUNCTION& f)
    {
      clear();

      if (!f.empty()) {
#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
       const impl_type* other_impl =
         reinterpret_cast<const impl_type*>(f.impl);
       impl = static_cast<void*>(other_impl->clone());
#  else
        invoker = f.invoker;
        manager = f.manager;
        functor = f.manager?
                    f.manager(f.functor, detail::function::clone_functor) : f.functor;
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      }
    }

  protected:
#if defined BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING || defined BOOST_NO_DEPENDENT_BASE_LOOKUP

#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
   // Clone a function pointer
   template<typename FunctionPtr>
   void* clone_functor(FunctionPtr f, detail::function::function_ptr_tag)
   {
     typedef typename detail::function::BOOST_FUNCTION_GET_FUNCTION_INVOKER<
                                FunctionPtr,
                                Allocator,
                                R BOOST_FUNCTION_COMMA
                                BOOST_FUNCTION_TEMPLATE_ARGS
                              >::type
         invoker_type;

#    ifdef BOOST_NO_STD_ALLOCATOR
     impl_type* i = new invoker_type(f);
#    else
     typedef typename Allocator::template rebind<invoker_type>::other
       allocator_type;
     typedef typename allocator_type::pointer pointer_type;
     allocator_type allocator;

     pointer_type copy = allocator.allocate(1);
     new (copy) invoker_type(f);
     impl_type* i = static_cast<impl_type*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
     return static_cast<void*>(i);
   }

   // Clone a function object
   template<typename FunctionObj>
   void* clone_functor(FunctionObj& f, detail::function::function_obj_tag)
   {
     typedef typename detail::function::BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER<
                                FunctionObj,
                                Allocator,
                                R BOOST_FUNCTION_COMMA
                                BOOST_FUNCTION_TEMPLATE_ARGS
                              >::type
         invoker_type;

#    ifdef BOOST_NO_STD_ALLOCATOR
     impl_type* i = new invoker_type(f);
#    else
     typedef typename Allocator::template rebind<invoker_type>::other
       allocator_type;
     typedef typename allocator_type::pointer pointer_type;
     allocator_type allocator;

     pointer_type copy = allocator.allocate(1);
     new (copy) invoker_type(f);
     impl_type* i = static_cast<impl_type*>(copy);
#    endif // BOOST_NO_STD_ALLOCATOR
     return static_cast<void*>(i);
   }
#  else
    // Clone a function pointer
    template<typename FunctionPtr>
    detail::function::any_pointer clone_functor(FunctionPtr f, detail::function::function_ptr_tag)
    {
      detail::function::any_pointer a(reinterpret_cast<void (*)()>(f));
      return manager(a, detail::function::clone_functor);
    }

    // Clone a functor
    template<typename FunctionObj>
    detail::function::any_pointer clone_functor(FunctionObj& f, detail::function::function_obj_tag)
    {
      detail::function::any_pointer a(static_cast<void*>(&f));
      return manager(a, detail::function::clone_functor);
    }
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS

    // Assign this BOOST_FUNCTION_FUNCTION to the given functor
    template<typename Functor>
    void assign_to(Functor f)
    {
      // Do we have a functor or a function pointer?
      typedef typename detail::function::IF<(is_pointer<Functor>::value),
                                  detail::function::function_ptr_tag,
                                  detail::function::function_obj_tag>::RET
        target_type;

#  ifdef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
      impl = clone_functor(f, target_type());
#  else
      typedef typename detail::function::IF<(is_pointer<Functor>::value),
                         detail::function::BOOST_FUNCTION_GET_FUNCTION_INVOKER<
                           Functor,
                           Allocator,
                           R BOOST_FUNCTION_COMMA
                           BOOST_FUNCTION_TEMPLATE_ARGS
                         >,
                         detail::function::BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER<
                           Functor,
                           Allocator,
                           R BOOST_FUNCTION_COMMA
                           BOOST_FUNCTION_TEMPLATE_ARGS
                         >
                       >::type
        invoker_selector;

      typedef typename invoker_selector::type invoker_type;

      // Setup the new functor
      invoker = &invoker_type::invoke;
      manager = &detail::function::functor_manager<Functor, Allocator>::manage;
      functor = clone_functor(f, target_type());
#  endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
    }
#endif // BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING

#ifndef BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
    typedef result_type (*invoker_type)(detail::function::any_pointer,
                                        bool BOOST_FUNCTION_COMMA
                                        BOOST_FUNCTION_TEMPLATE_ARGS
                                       );

    invoker_type invoker;
#endif // BOOST_FUNCTION_USE_VIRTUAL_FUNCTIONS
  };

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

  template<typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS ,
           typename Policy, typename Mixin, typename Allocator>
  inline void swap(BOOST_FUNCTION_FUNCTION<
                     R BOOST_FUNCTION_COMMA
                     BOOST_FUNCTION_TEMPLATE_ARGS ,
                     Policy,
                     Mixin,
                     Allocator
                   >& f1,
                   BOOST_FUNCTION_FUNCTION<
                     R BOOST_FUNCTION_COMMA 
                     BOOST_FUNCTION_TEMPLATE_ARGS,
                     Policy,
                     Mixin,
                     Allocator
                   >& f2)
  {
    f1.swap(f2);
  }
                   
                   
}
