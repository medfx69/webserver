#include "pars.hpp"

std::vector<server> Parsed::getDate() const
{
	return handled_data;
}

Parsed::Parsed(char *file)
{
	s = parec(file);
	handled_data = data_handler(s);
	
}

Parsed &Parsed::operator=(const Parsed &parsed)
{
	for (size_t i = 0; i < parsed.getDate().size() ; i++)
	{
		handled_data.push_back(server());
		handled_data[i] = parsed.getDate()[i];
	}
	return *this;
}
Parsed::Parsed(const Parsed &s)
{
	// this->s = new data_reader();
	*this = s;
}
Parsed::Parsed()
{
	// s = new data_reader();
}

std::vector<data_reader> Parsed::getserver() const
{
	return s;
}
// Parsed::~Parsed()
// {
// 	// delete s;
// }