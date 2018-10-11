#include "fluentd-logging.hpp"

using namespace std;

/* Constructors
 */
FluentdLogging::FluentdLogging()
{
    mLogger = nullptr;
    mFluentdHooked = false;
    mHost = "";
    mPort = 0;
}
FluentdLogging::FluentdLogging(std::string host, int port)
{
    mLogger = nullptr;
    mFluentdHooked = false;
    mHost = host;
    mPort = port;
}
FluentdLogging::~FluentdLogging()
{
    delete mLogger;
}

/* InitLogging instantiates the logger if the enviroment
 * variables are set.
 */
void FluentdLogging::InitLogging()
{
    if (mHost.length() > 0 && mPort != 0)
    {
        mFluentdHooked = true;
        mLogger = new fluent::Logger();
        mLogger->new_forward(mHost, mPort);
    }
    else
    {
        LogMessage("",
            R"({"WARN":{"Process":"InitLogging","Message":"No fluend connection info provided."}})"_json);
    }
}

/* Will return current time.
 */
std::string FluentdLogging::getStamp()
{
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "%Y-%m-%dT%H-%M-%SZ", timeinfo);
    return buffer;
}

/* LogMessage creates a logging message to print to std out or
 * fluentd if it's hooked.
 */
void FluentdLogging::LogMessage(string version, nlohmann::json msg)
{
    string stamp = getStamp();
    if (msg.find("ERROR") != msg.end())
    {
        nlohmann::json err;
        err = msg["ERROR"];
        err["ErrorTime"] = stamp;
        err["Version"] = version;
        msg["ERROR"] = err;
        mErrArray.push_back(err);
    }
    if (msg.find("WARN") != msg.end())
    {
        nlohmann::json warn;
        warn = msg["WARN"];
        warn["WarnTime"] = stamp;
        warn["Version"] = version;
        msg["WARN"] = warn;
        mWarnArray.push_back(warn);
    }
    if (mFluentdHooked)
    {
        fluent::Message* fluentMsg = mLogger->retain_message("Logging");
        fluentMsg->set("Log_Message", msg.dump());
        mLogger->emit(fluentMsg);
    }
    cout << msg.dump() << endl;
}

/* LogErrors curates the catalog record with any errors that happened
 * throughout the generic extraction process. Otherwise set to null.
 */
nlohmann::json FluentdLogging::LogErrors(nlohmann::json raw)
{
    if (mErrArray.size() != 0)
    {
        raw["errors"] = mErrArray;
    }
    return raw;
}