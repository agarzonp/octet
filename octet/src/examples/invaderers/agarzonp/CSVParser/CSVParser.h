//
// Note: The core parsing in CsvParser::ParseFile is from https://github.com/andy-thomason/read_a_csv_file  
// I just added some modifications to be more reusable.
//

#ifndef CSV_PARSER
#define CSV_PARSER

#include <fstream>
#include <vector>
#include <cassert>

namespace agarzonp
{
	class CSVRow
	{
		std::vector<std::string> tokens;

	public:

		void AddToken(const char* start, const char* end)
		{
			tokens.emplace_back(start, end);
		}
		
		const char* operator[] (size_t index) const
		{
			assert(index < tokens.size());

			if (index < tokens.size())
			{
				return tokens[index].c_str();
			}

			return "";
		}

		const size_t NumTokens() const { return tokens.size(); }
	};


	class CSVParser
	{
		const char* filePath;
		bool isValid;

		typedef std::vector<CSVRow> CSVRows;
		CSVRows rows;

	public:

		CSVParser(const char* file)
			: filePath(file)
			, isValid(false)
		{
			ParseFile(filePath);
		}

		~CSVParser()
		{
		}

		const CSVRows& Rows() const { return rows; }

		bool IsValid() { return isValid; }

		const CSVRow& operator[](size_t rowIndex)
		{
			assert(rowIndex < rows.size());
			return rows[rowIndex];
		}

		size_t NumRows() { return rows.size();  }
		size_t NumCols() { return rows.size() > 0 ? rows[0].NumTokens() : 0; }

	private:

		void ParseFile(const char* file)
		{
			std::ifstream is(file);

			if (is.bad() || !is.is_open())
			{
				return;
			}

			isValid = true;

			// store the line here
			char buffer[2048];

			// loop over lines
			while (!is.eof()) 
			{
				is.getline(buffer, sizeof(buffer));

				CSVRow row;

				char *b = buffer;
				if (*b != '\0') // avoid the empty line
				{
					// loop over columns
					for (int col = 0; ; ++col) {
						char *e = b;
						while (*e != 0 && *e != ',') ++e;

						// now b -> e contains the chars in a column
						row.AddToken(b, e);

						if (*e != ',') break;
						b = e + 1;
					}

					rows.emplace_back(row);
				}
			}
		}
	};
}

#endif // !CSV_PARSER