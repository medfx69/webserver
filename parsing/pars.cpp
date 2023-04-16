#include "pars.hpp"

server *Parsed::getDate() const
{
	return handled_data;
}

Parsed::Parsed(char *file)
{
	s = parec(file);
	handled_data = data_handler(s);
	pars_locations(this);
}

Parsed &Parsed::operator=(const Parsed &parsed)
{
	data_reader tmp;
	std::vector<data_reader>::iterator it = parsed.getserver()->block.begin();

	delete s;
	s = new data_reader();
	while (it < parsed.getserver()->block.end())
	{
		s->block.push_back(*it);
		it++;
	}
	s->block_name = parsed.getserver()->block_name;
	std::vector<std::string>::iterator it2;
	for (it2 = parsed.getserver()->dir.begin(); it2 < parsed.getserver()->dir.end(); it2++)
	{
		s->dir.push_back(*it2);
	}
	return *this;
}
Parsed::Parsed(const Parsed &s)
{
	this->s = new data_reader();
	*this = s;
}
Parsed::Parsed()
{

	s = new data_reader();
}

data_reader *Parsed::getserver() const
{
	return s;
}
Parsed::~Parsed()
{
	delete s;
}