g++ -std=c++17 -Wall -Wextra -Werror -pedantic -I. -Iinclude \
  Resource.cpp ResourceManager.cpp src/Reservation.cpp \
  tests/final_resource_management_tests.cpp -o final_resource_management_tests
./final_resource_management_tests 
