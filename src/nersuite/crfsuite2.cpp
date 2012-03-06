#include "crfsuite2.h"
#include "crfsuite.hpp"

using namespace CRFSuite;

size_t	Trainer2::get_attribute_num() const
{
	if (data != NULL)
	{
		return data->attrs->num(data->attrs);
	}
	return 0;
}

size_t	Trainer2::get_label_num() const
{
	if (data != NULL)
	{
		return data->labels->num(data->labels);
	}
	return 0;
}

size_t	Trainer2::get_instance_num() const
{
	if (data != NULL)
	{
		return data->num_instances;
	}
	return 0;
}

size_t	Trainer2::get_item_num() const
{
	if (data != NULL)
	{
		return static_cast<size_t>(crfsuite_data_totalitems(data));
	}
	return 0;
}

void Trainer2::message(const std::string& msg)
{
	std::cerr << msg;
}
