#ifndef CPP_FLUENTD_LOGGING
#define CPP_FLUENTD_LOGGING

#include <string>
#include "fluent.hpp"
#include "nlohmann/json.hpp"

class FluentdLogging
{
public:
    /* Constructors
     */
    FluentdLogging();
    FluentdLogging(std::string host, int port);
    ~FluentdLogging();

    /* InitLogging instantiates the logger if the enviroment
     * variables are set.
     */
    void InitLogging();

    /* Will return current time.
     */
    std::string getStamp();

    /* LogMessage creates a logging message to print to std out or
     * fluentd if it's hooked.
     */
    void LogMessage(std::string version, nlohmann::json msg);

    /* LogErrors curates the catalog record with any errors that happened
     * throughout the generic extraction process. Otherwise set to null.
     */
    nlohmann::json LogErrors(nlohmann::json raw);

    std::string getFluentdHost()
    {
        return mHost;
    }
    int getFluentdPort()
    {
        return mPort;
    }
    fluent::Logger* getFluentdLogger()
    {
        return mLogger;
    }

private:
    std::string mHost;
    int mPort;
    fluent::Logger* mLogger;
    bool mFluentdHooked;
    nlohmann::json mErrArray;
    nlohmann::json mWarnArray;
};
#endif