/**
 * @file macro.h
 * @author Daniel Starke
 * @copyright Copyright 2022 Daniel Starke
 * @date 2022-02-06
 * @version 2022-03-23
 * 
 * @warning This file is for internal use only. The content is subject to change at any time.
 * @remarks This file requires C++11 or newer.
 */
#ifndef __SCDINTERNAL_MACRO_H__
#define __SCDINTERNAL_MACRO_H__


/** C++ STL void_t re-implementation. */
template <typename ...>
using void_t = void;


/** Simple parameter pack for template operations. */
template <typename ...>
struct parameter_pack {};


/**
 * Primary function to assign a value to a member field.
 * 
 * @param[in] int - needed to prioritize this function during function signature resolution
 * @param[in] fn - functor which handles the assignment
 * @param[in,out] var - variable which contains the member field
 * @param[in] val - new value
 * @tparam F - functor with this call signature void Fn(auto & var, const auto value) -> void
 */
template <typename F, typename T0, typename T1>
static inline auto set_for_existing_member(const int, const F && fn, T0 & var, const T1 val) -> decltype(fn(var, val)) {
	fn(var, val);
}

/**
 * Secondary function to assign a value to a member field.
 * This function is being invoked if the passed functor is invalid, i.e.
 * the variable has no such member field.
 * 
 * @param[in] float - fallback during function signature resolution by implicit cast from int to float
 * @param[in] fn - functor which handles the assignment
 * @param[in,out] var - variable which contains the member field
 * @param[in] val - new value
 * @tparam F - functor with this call signature void Fn(auto & var, const auto value) -> void
 */
template <typename F, typename T0, typename T1>
static inline auto set_for_existing_member(const float, const F &&, T0 &, const T1) -> void {
	/* no operation */
}


/**
 * Macro to assign a value to the member field of a passed variable
 * only if the variable has such a member field.
 * 
 * @param[in,out] variable - owning variable of the member field
 * @param[in] member - name of the member field to use
 * @param[in] value - new value
 * @remarks `SET_FOR_EXISTING_MEMBER(variable, member, value);` reads as `variable.member = value;`
 */
#define SET_FOR_EXISTING_MEMBER(variable, member, value) \
	set_for_existing_member( \
		0 /* prefer int here */, \
		[](auto & var, const decltype(var.member) val) -> void { var.member = val; }, \
		(variable), \
		(value) \
	)


/**
 * Macro to assign a value to the indirected member field of a passed variable
 * only if the variable has such a member field.
 * 
 * @param[in,out] variable - owning variable of the indirected member field
 * @param[in] member - name of the member field to use
 * @param[in] value - new value
 * @remarks `SET_FOR_EXISTING_MEMBER_PTR(variable, member, value);` reads as `variable->member = value;`
 */
#define SET_FOR_EXISTING_MEMBER_PTR(variable, member, value) \
	set_for_existing_member( \
		0 /* prefer int here */, \
		[](auto & var, const decltype(var->member) val) -> void { var->member = val; }, \
		(variable), \
		(value) \
	)


/**
 * Primary function to call a function with the value of an enumeration field.
 * 
 * @param[in] int - needed to prioritize this function during function signature resolution
 * @param[in] fn0 - functor which is called with the enumeration field value
 * @param[in] fn1 - functor which handles the enumeration field value resolution
 * @param[in] val - random value of the requested enumeration type
 * @tparam F0 - functor with this call signature void Fn(auto val) -> decltype(val)
 * @tparam F1 - functor with this call signature void Fn(auto val) -> void
 */
template <typename F0, typename F1, typename T>
static inline auto call_for_existing_field(const int, const F0 & fn0, const F1 && fn1, const T val) -> void_t<decltype(fn0(fn1(val)))> {
	fn0(fn1(val));
}


/**
 * Secondary function to call a function with the value of an enumeration field.
 * 
 * @param[in] float - fallback during function signature resolution by implicit cast from int to float
 * @param[in] fn0 - functor which is called with the enumeration field value
 * @param[in] fn1 - functor which handles the enumeration field value resolution
 * @param[in] val - random value of the requested enumeration type
 * @tparam F0 - functor with this call signature void Fn(auto val) -> decltype(val)
 * @tparam F1 - functor with this call signature void Fn(auto val) -> void
 */
template <typename F0, typename F1, typename T>
static inline auto call_for_existing_field(const float, const F0 &, const F1 &&, const T) -> void {
	/* no operation */
}


/**
 * Macro to call a given function with the passed enumeration field only if the
 * enumeration has such a field.
 * 
 * @param[in] function - function which is called with the enumeration field value
 * @param[in] enumeration - enumeration type name
 * @param[in] field - enumeration field name
 */
#define CALL_FOR_EXISTING_FIELD(function, enum, field) \
	call_for_existing_field( \
		0 /* prefer int here */, \
		(function), \
		[](auto val) -> decltype(decltype(val)::field) { return decltype(val)::field; }, \
		static_cast<enum>(0) \
	)


/**
 * Macro to call a functor or lambda which accepts a parameter of type `enum`
 * only if the enumeration has a `field` of the given name.
 * 
 * @param[in] name - wrapper function name
 * @param[in] enum - enumeration type name
 * @param[in] field - enumeration field name
 */
#define DEF_CALL_FOR_EXISTING_FIELD(name, enum, field) \
	template <typename Fn, typename T> \
	static inline auto name##_(const int, Fn && fn) -> decltype(fn(T::field)) { \
		fn(T::field); \
	} \
	template <typename Fn, typename T> \
	static inline auto name##_(const float, Fn &&) -> void { \
		/* no operation */ \
	} \
	template <typename Fn> \
	static inline void name(Fn && fn) { \
		name##_<Fn, enum>(0 /* prefer int here */, fn); \
	}


/**
 * Defines the function wrapper `name` which the given return value type.
 * This calls `fn` or return `def` in case that `fn` has not been declared.
 * 
 * @param ret - return value
 * @param name - wrapper function name
 * @param fn - name of the function
 * @param def - default return value
 */
#define DEF_NO_FN_WRAPPER(ret, name, fn, def) \
	template <typename ...Ts> \
	static inline auto name##_Case(const int, Ts... args) -> decltype(fn(args...)) { \
		return fn(args...); \
	} \
	template <typename ...Ts> \
	static inline auto name##_Case(const float, Ts...) -> ret { \
		return def; \
	} \
	template <typename ...Ts> \
	static inline ret name(Ts... args) { \
		return name##_Case(0 /* prefer int here */, args...); \
	}


/**
 * Defines the function wrapper `name` which the given return value type.
 * This calls `fn` or return `def` in case that `fn` has not been declared.
 * Explicit function argument types are used.
 * 
 * @param ret - return value
 * @param name - wrapper function name
 * @param fn - name of the function
 * @param def - default return value
 * @param ... - function argument types
 */
#define DEF_NO_FN_ARGS_WRAPPER(ret, name, fn, def, ...) \
	template <typename ...Ts> \
	static inline auto name##_Case(const int, Ts... args) -> decltype(fn(args...)) { \
		return fn(args...); \
	} \
	template <typename ...Ts> \
	static inline auto name##_Case(const float, Ts...) -> ret { \
		return def; \
	} \
	template <typename ...Ts> \
	static inline ret name##_Switch(Ts... args) { \
		return name##_Case(0 /* prefer int here */, args...); \
	} \
	static constexpr auto name = name##_Switch< __VA_ARGS__ >;


/**
 * Defines the function wrapper `name` which the given return value type.
 * This calls `fn1` and falls back to `fn2` in case that `fn1` has not been
 * declared.
 * 
 * @param ret - return value
 * @param name - wrapper function name
 * @param fn1 - name of the primary function
 * @param fn2 - name of the secondary function
 */
#define DEF_ALT_FN_WRAPPER(ret, name, fn1, fn2) \
	template <typename ...Ts> \
	static inline auto name##_Case(const int, Ts... args) -> decltype(fn1(args...)) { \
		return fn1(args...); \
	} \
	template <typename ...Ts> \
	static inline auto name##_Case(const float, Ts... args) -> decltype(fn2(args...)) { \
		return fn2(args...); \
	} \
	template <typename ...Ts> \
	static inline ret name(Ts... args) { \
		return name##_Case(0 /* prefer int here */, args...); \
	}


/**
 * Defines the function wrapper `name` which the given return value type.
 * This calls `fn1` and falls back to `fn2` in case that `fn1` has not been
 * declared. Explicit function argument types are used.
 * 
 * @param ret - return value
 * @param name - wrapper function name
 * @param fn1 - name of the primary function
 * @param fn2 - name of the secondary function
 * @param ... - function argument types
 */
#define DEF_ALT_FN_ARGS_WRAPPER(ret, name, fn1, fn2, ...) \
	template <typename ...Ts> \
	static inline auto name##_Case(const int, Ts... args) -> decltype(fn1(args...)) { \
		return fn1(args...); \
	} \
	template <typename ...Ts> \
	static inline auto name##_Case(const float, Ts... args) -> decltype(fn2(args...)) { \
		return fn2(args...); \
	} \
	template <typename ...Ts> \
	static inline ret name##_Switch(Ts... args) { \
		return name##_Case(0 /* prefer int here */, args...); \
	} \
	static constexpr auto name = name##_Switch< __VA_ARGS__ >;


#endif /* __SCDINTERNAL_MACRO_H__ */
