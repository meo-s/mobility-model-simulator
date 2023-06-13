#pragma once
#ifndef _TYPE_TRAITS_HH
#define _TYPE_TRAITS_HH

#include <type_traits>

template <typename T>
struct remove_cv_ref : public std::remove_const<std::remove_reference<T>> {};

template <typename T>
using remove_cv_ref_t = typename remove_cv_ref<T>::type;

template <typename TypeFrom, typename TypeTo>
struct propagate_const
    : public std::conditional<std::is_const_v<std::remove_reference_t<TypeFrom>>, std::add_const_t<TypeTo>, TypeTo> {};

template <typename TypeFrom, typename TypeTo>
using propagate_const_t = typename propagate_const<TypeFrom, TypeTo>::type;

template <typename T, template <typename...> typename Template_>
struct _is_template_instance : public std::false_type {};

template <typename... TArgs, template <typename...> typename Template_>
struct _is_template_instance<Template_<TArgs...>, Template_> : public std::true_type {};

template <typename T, template <typename...> typename Template_>
struct is_template_instance : public _is_template_instance<remove_cv_ref_t<T>, Template_> {};

template <typename T, template <typename...> typename Template_>
constexpr bool is_template_instance_v = is_template_instance<T, Template_>::value;

#endif
