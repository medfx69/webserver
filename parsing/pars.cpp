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
	// data_reader tmp;
	// std::vector<data_reader>::iterator it = this->getserver()->block.begin();

	// delete s;
	// s = new data_reader();
	// while (it < parsed.getserver()->block.end())
	// {
	// 	s->block.push_back(*it);
	// 	it++;
	// }
	// s->block_name = parsed.getserver()->block_name;
	// std::vector<std::string>::iterator it2;
	// for (it2 = parsed.getserver()->dir.begin(); it2 < parsed.getserver()->dir.end(); it2++)
	// {
	// 	s->dir.push_back(*it2);
	// }
	// this->handled_data = parsed.getDate();
	for (std::vector<server>::iterator it = parsed.getDate().begin(); it < parsed.getDate().end(); it++)
	{
		this->handled_data.push_back(*it);
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