
#include <fstream>
#include "fluentd-logging.hpp"
#include "nlohmann/json.hpp"

#define VERSION "0.0.1"

using namespace std;

FluentdLogging logger;

/* Will take a passed JSON object and write to a file.
 */
bool writeJsonFile(string filename, nlohmann::json jo)
{
    // log errors to output
    jo = logger.LogErrors(jo);

    ofstream jsonFile(filename);
    if (jsonFile.is_open())
    {
        jsonFile << jo.dump(2) << "\n";
        jsonFile.close();
    }
    else
    {
        return false;
    }
    return true;
}

/* Will merge 2 json objects.  Fields from the "b" object will overwrite fields in the "a" object of
 * the same name.
*
*/
nlohmann::json merge(const nlohmann::json& a, const nlohmann::json& b)
{
    nlohmann::json result = a.flatten();
    nlohmann::json tmp = b.flatten();
    for (auto it = tmp.begin(); it != tmp.end(); ++it)
        result[it.key()] = it.value();
    return result.unflatten();
}

/* Returns string of json from file.
 */
string readJsonFile(string filename)
{
    logger.LogMessage(VERSION, R"({"INFO":"Attempting to read json file."})"_json);
    try
    {
        ifstream file(filename);
        stringstream buffer;
        buffer << file.rdbuf();
        logger.LogMessage(VERSION, R"({"INFO":"Json file imported."})"_json);
        return buffer.str();
    }
    catch (...)
    {
        logger.LogMessage(VERSION, R"({"ERROR":"Failed to read json file."})"_json);
    }
    return "";
}

/* Checks if file exists.
 */
bool fexists(const char* filename)
{
    ifstream ifile(filename);
    return ifile.good();
}

/* Driver code...
 */
int main(int argc, char* argv[])
{
    nlohmann::json jo;

    if (getenv("GUEST_INPUT_FILE") == NULL || getenv("GUEST_OUTPUT_FILE") == NULL)
    {
        logger.LogMessage(VERSION, R"({"ERROR":"Missing input and/or output arguments."})"_json);
        return -1;
    }

    bool fexistsFlag = false;

    if (!fexists(getenv("GUEST_INPUT_FILE")))
    {
        logger.LogMessage(VERSION, R"({"ERROR":"Input file not found."})"_json);
        fexistsFlag = false;
    }
    else
    {
        logger.LogMessage(VERSION, R"({"INFO":"Input file found."})"_json);
        fexistsFlag = true;
    }

    if (!fexists(getenv("GUEST_OUTPUT_FILE")))
    {
        logger.LogMessage(VERSION, R"({"ERROR":"Output file not found."})"_json);
        fexistsFlag = false;
    }
    else
    {
        logger.LogMessage(VERSION, R"({"INFO":"Output file found."})"_json);
        fexistsFlag = true;
    }

    if (!fexistsFlag)
    {
        logger.LogMessage(VERSION, R"({"ERROR":"Please verify input output files."})"_json);
        return -1;
    }

    logger.LogMessage(VERSION, R"({"INFO":"Ready to parse input file."})"_json);
    string input = readJsonFile(getenv("GUEST_INPUT_FILE"));

    logger.LogMessage(VERSION, R"({"INFO":"Ready to parse output file."})"_json);
    string output = readJsonFile(getenv("GUEST_OUTPUT_FILE"));

    if (input == "" || output == "")
    {
        logger.LogMessage(VERSION,
            R"({"ERROR":"An unknown error occurred while parsing input and output files."})"_json);
        return -1;
    }

    nlohmann::json jInput;
    logger.LogMessage(VERSION, R"({"INFO":"Converting input string to json"})"_json);
    jInput = nlohmann::json::parse(input);
    if (jInput == NULL)
    {
        logger.LogMessage(VERSION, R"({"ERROR":"Failed to convert input string to json"})"_json);

        return -1;
    } else {
        cout << jInput.dump(2) << endl;
    }

    nlohmann::json jOutput;
    logger.LogMessage(VERSION, R"({"INFO":"Converting output string to json"})"_json);

    jOutput = nlohmann::json::parse(output);
    if (jOutput == NULL)
    {
        logger.LogMessage(VERSION, R"({"ERROR":"Failed to convert output string to json"})"_json);
        return -1;
    } else {
        cout << jOutput.dump(2) << endl;
    }

    logger.LogMessage(VERSION,
        R"({"INFO":"Merging the input json object into the output json object.  Fields from the input object will overwrite fields in the output object of the same name."})"_json);
    jOutput = merge(jOutput, jInput);
    cout << jOutput.dump(2) << endl;

    writeJsonFile(getenv("GUEST_OUTPUT_FILE"), jOutput);

    logger.LogMessage(VERSION, R"({"INFO":"Merge complete"})"_json);
}