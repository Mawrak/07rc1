#pragma once

template <class T, class... TArgs>
IC T* xr_new(TArgs&&... args)
{
	T* ptr = (T*)Memory.mem_alloc(sizeof(T));

	T* newly_created = new (ptr) T(std::forward<TArgs>(args)...);

	return newly_created;
}

template <bool _is_pm, typename T>
struct xr_special_free
{
	IC void operator()(T*& ptr)
	{
		void* _real_ptr = dynamic_cast<void*>(ptr);
		ptr->~T();
		xr_free(_real_ptr);
	}

	IC void operator()(T* const& ptr)
	{
		void* _real_ptr = dynamic_cast<void*>(ptr);
		ptr->~T();
		xr_free(_real_ptr);
	}
};

template <typename T>
struct xr_special_free<false, T>
{
	IC void operator()(T*& ptr)
	{
		ptr->~T();
		xr_free(ptr);
	}
};

template <class T>
IC void xr_delete(T*& ptr)
{
	if (ptr)
	{
		xr_special_free<::is_polymorphic<T>::result, T>()(ptr);
		ptr = NULL;
	}
}

template <class T>
IC void xr_delete(T* const& ptr)
{
	if (ptr)
	{
		xr_special_free<::is_polymorphic<T>::result, T>()(ptr);
		((T*&)ptr) = NULL;
	}
}
