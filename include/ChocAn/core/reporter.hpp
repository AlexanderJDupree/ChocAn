/* 
 
File: reporter.hpp

Brief:  reporter service makes structured queries to the databse to compile 
        different types of reports

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_REPORTER_HPP
#define CHOCAN_REPORTER_HPP

#include <ChocAn/core/data_gateway.hpp>
#include <ChocAn/core/entities/datetime.hpp>
#include <ChocAn/core/entities/account_report.hpp>

class Reporter
{
public:

    using Database_Ptr = Data_Gateway::Database_Ptr;

    Reporter(Database_Ptr db);

    Summary_Report gen_summary_report(const DateTime& start, const DateTime& end) const;

    Provider_Report gen_provider_report(const DateTime& start, const DateTime& end, const Account& provider) const;

    Member_Report gen_member_report(const DateTime& start, const DateTime& end, const Account& member) const;

private:

    Database_Ptr db;

};


#endif // CHOCAN_REPORTER_HPP