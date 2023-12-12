#include "assets.h"
#include "log.h"

#include <cute.h>

void mount_assets_folder()
{
	Path path = cf_fs_get_base_directory();
	path.normalize();

	path += "assets";
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

Array<Array<String>> parse_csv_str(const char *str)
{
	String s = String(str);

	Array<Array<String>> table;

	for (String &row : s.split('\n'))
	{
		table.add(parse_csv_row(row));
	}

	return table;
}

Array<Array<String>> parse_csv_file(const char *path)
{
	const char *csv =
		cf_fs_read_entire_file_to_memory_and_nul_terminate(path, nullptr);

	return parse_csv_str(csv);
}
