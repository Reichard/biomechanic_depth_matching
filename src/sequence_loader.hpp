#pragma once

#include <future>

template<class T>
class SequenceLoader {
	public:
		using item_type = T;
		using index_type = int;

		SequenceLoader() = default;
		virtual ~SequenceLoader() = default;

		item_type load(index_type index) {
			//retreive prefeteched item if it has the correct index
			item_type item = index == _next_index 
				? _next_item.get() 
				: load_item(index);

			//prefetch next item
			_next_index = index+1;
			_next_item = std::async(std::launch::async, [=]{ return load_item(index+1); });

			return item;
		}

	protected:
		virtual item_type load_item(index_type index) = 0;

	private:
		index_type _next_index = -1;
		std::future<item_type> _next_item;
};
