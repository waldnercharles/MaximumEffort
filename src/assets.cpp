#include "assets.h"
#include "log.h"

#include <cute.h>

void mount_assets_folder()
{
	Path path = cf_fs_get_base_directory();
	path.normalize();

	path += "../assets";
	cf_fs_mount(path.c_str(), "/", true);

	log_info("{} mounted to \"/\"", path.c_str());
}

enum CSVState
{
	UnquotedField,
	QuotedField,
	QuotedQuote
};

Array<String> parse_csv_row(const String &row)
{
	CSVState state = CSVState::UnquotedField;
	Array<String> fields {""};
	int i = 0; // index of the current field
	for (char c : row)
	{
		switch (state)
		{
			case CSVState::UnquotedField:
				switch (c)
				{
					case ',': // end of field
						fields.add("");
						i++;
						break;
					case '"':
						state = CSVState::QuotedField;
						break;
					default:
						fields[i].add(c);
						break;
				}
				break;
			case CSVState::QuotedField:
				switch (c)
				{
					case '"':
						state = CSVState::QuotedQuote;
						break;
					default:
						fields[i].add(c);
						break;
				}
				break;
			case CSVState::QuotedQuote:
				switch (c)
				{
					case ',': // , after closing quote
						fields.add("");
						i++;
						state = CSVState::UnquotedField;
						break;
					case '"': // "" -> "
						fields[i].add('"');
						state = CSVState::QuotedField;
						break;
					default: // end of quote
						state = CSVState::UnquotedField;
						break;
				}
				break;
		}
	}
	return fields;
}

Array<Array<String>> parse_csv(const char *str)
{
	String s = String(str);

	Array<Array<String>> table;

	for (String row : s.split('\n'))
	{
		table.add(parse_csv_row(row));
	}

	return table;
}

//Array<Array<const char *>> parse_csv(const char *str)
//{
//	Array<Array<const char *>> parsed = {};
//
//	char *row = nullptr;
//	char *row_copy = nullptr;
//
//	char *cell = nullptr;
//
//	row_copy = cf_sset(row_copy, str);
//	while ((row = cf_ssplit_once(row_copy, '\n')))
//	{
//		auto &arr = parsed.add();
//		char *cell_copy = nullptr;
//		cell_copy = cf_sset(cell_copy, row);
//		while ((cell = cf_string_split_once(cell_copy, ',')))
//		{
//			arr.add(cell);
//		}
//		arr.add(cell_copy); // Add the last cell
//	}
//
//	// Add the last row
//	auto &arr = parsed.add();
//	char *cell_copy = nullptr;
//	cell_copy = cf_sset(cell_copy, row_copy);
//	while ((cell = cf_string_split_once(row_copy, ',')))
//	{
//		arr.add(cell);
//	}
//	arr.add(cell_copy); // Add the last cell
//
//	return parsed;
//}