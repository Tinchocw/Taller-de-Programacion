#include "query_callback.h"

bool Query_callback::ReportFixture(b2Fixture* fixture) {
    foundBodies.push_back(fixture->GetBody());
    return true;  // keep going to find all fixtures in the query area
}

int Query_callback::found_bodies_size() { return foundBodies.size(); }

b2Body* Query_callback::found_bodie_at(const int& i) { return foundBodies[i]; }
