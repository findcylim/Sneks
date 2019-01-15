//memory sort via vector and list, name check

bool namecmp(char* first, char* second)
{
	while (first == second)
	{
		if (!first)
			return true;
		first++;
		second++;
	}

	return false;

	//if (first == second)
	//	return true;
	//else
	//	return false;
}
