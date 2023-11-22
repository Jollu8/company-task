//
/// Created by Jollu Emil
//

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "Server.h"

#include <cassert>
#include <unordered_map>

void TestCalculateSHA1() {
    assert(CalculateSHA1("test") == "a94a8fe5ccb19ba61c4c0873d391e987982fbbd3");
    assert(CalculateSHA1("hello world") == "2aae6c35c94fcfb415dbe95f408b9ce91ee846ed");
}

void TestCountHits() {
    std::unordered_map<std::string, int> hit_count_Map;
    CountHits("path1", hit_count_Map);
    assert(hit_count_Map["path1"] == 1);
    CountHits("path1", hit_count_Map);
    assert(hit_count_Map["path1"] == 2);
    CountHits("path2", hit_count_Map);
    assert(hit_count_Map["path2"] == 1);
}

void TestExtractPath() {
    assert(ExtractPath("GET /path1 HTTP/1.1\r\nHost: localhost\r\n\r\n") == "/path1");
    assert(ExtractPath("GET /path2 HTTP/1.1\r\nHost: localhost\r\n\r\n") == "/path2");
}

void TestExtractUserAgent() {
    assert(ExtractUserAgent("GET / HTTP/1.1\r\nUser-Agent: agent1\r\n\r\n") == "agent1");
    assert(ExtractUserAgent("GET / HTTP/1.1\r\nUser-Agent: agent2\r\n\r\n") == "agent2");
}

int main() {
    TestCalculateSHA1();
    TestCountHits();
    TestExtractPath();
    TestExtractUserAgent();
    std::cout << "All tests passed.\n";
    return 0;
}
