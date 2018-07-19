#include <cmath>
#include <iostream>

#include "test.h"

#include "date.h"
#include "sun.h"

/* TODOs:
 * - ?? SetCalendarDate() -> SetTT()?
 * - Earth/Sun/...: Value caching (Pass required info by argument?)
 * - test_internal(): If verbose, also show expected values.
 */

int main(void)
{
    if(test_internal()) {
        std::cout << "Internal Test OK!" << std::endl;
    } else {
        std::cout << "Error: Internal Test Failed!" << std::endl;
    }

    return 0;
}
