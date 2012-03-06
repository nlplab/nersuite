#ifndef __CRFSUITE2_H
#define __CRFSUITE2_H

#include "crfsuite.h"
#include "crfsuite_api.hpp"

namespace CRFSuite
{

	class Trainer2 : public Trainer
	{
	public:
		size_t	get_attribute_num() const;

		size_t	get_label_num() const;

		size_t	get_instance_num() const;

		size_t	get_item_num() const;

		virtual void message(const std::string& msg);
	};

}
#endif
