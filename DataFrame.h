//
// Created by blakberrisigma on 12/07/2023.
//

#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <variant>
#include <iomanip>
#include "third_party/csv.hpp"

namespace qz {

    using cell_data_t = std::variant<int, float, double, std::string>;
    using data_row_t = std::unordered_map<std::string, cell_data_t>;

    // Convert cell data into std::string
    std::string cell_to_str(const cell_data_t &data) {
        return std::visit([](auto &&arg) -> std::string {
            std::stringstream ss;
            ss << arg;
            return ss.str();
        }, data);
    }

    // class DataFrame
    class DataFrame {
    public:
        DataFrame() = default;

        void append_row(const data_row_t &row) {
            m_records.push_back(row);
        }

        // Get the number of rows in the DataFrame
        [[nodiscard]] size_t row_count() const {
            return m_records.size();
        }

        // Get the number of columns in the DataFrame
        [[nodiscard]] size_t col_count() const {
            return m_records.empty() ? 0 : m_records[0].size();
        }

        // Get column names in the DataFrame
        [[nodiscard]] std::vector<std::string> columns() const {
            return m_column_names;
        }

        void read_csv(const std::string &filename) {
            csv::CSVReader reader(filename);
            m_column_names = reader.get_col_names();
            size_t col_idx = 0;

            for (csv::CSVRow &row: reader) {
                data_row_t dataRow;
                for (csv::CSVField &field: row) {
                    dataRow[m_column_names[col_idx++]] = field.get<>();
                }
                append_row(dataRow);
                col_idx = 0;
            }
        }

        // Overload the << operator
        friend std::ostream &operator<<(std::ostream &os, const DataFrame &df) {
            std::vector<std::string> columnNames = df.columns();

            // print column names
            for (const auto &columnName: columnNames) {
                os << std::setw(15) << std::left << columnName;
            }
            os << "\n" << std::string(columnNames.size() * 15, '-') << "\n";

            // print rows
            for (const auto &row: df.m_records) {
                for (const auto &columnName: columnNames) {
                    os << std::setw(15) << std::left << cell_to_str(row.at(columnName));
                }
                os << "\n";
            }
            return os;
        }

    private:
        std::vector<data_row_t> m_records;
        std::vector<std::string> m_column_names;
    };

} // qz

#endif //DATAFRAME_H
