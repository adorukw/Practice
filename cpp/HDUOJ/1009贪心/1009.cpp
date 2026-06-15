#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct RoomTrade {
    int javaBeans;
    int catFood;
    double priceRatio;
};

bool compareRoom(RoomTrade room1, RoomTrade room2) {
    return room1.priceRatio > room2.priceRatio;
}

double getMaxJavaBeans(int totalCatFood, vector<RoomTrade>& rooms) {
    sort(rooms.begin(), rooms.end(), compareRoom);
    
    double result = 0.0;
    int remainingFood = totalCatFood;
    
    for (const auto& room : rooms) {
        if (remainingFood <= 0) break;
        
        if (room.catFood <= remainingFood) {
            result += room.javaBeans;
            remainingFood -= room.catFood;
        } else {
            double ratio = static_cast<double>(remainingFood) / room.catFood;
            result += room.javaBeans * ratio;
            remainingFood = 0;
        }
    }
    
    return result;
}

int main() {
    int mouseCatFood, roomCount;
    
    while (cin >> mouseCatFood >> roomCount) {
        if (mouseCatFood == -1 && roomCount == -1) break;
        
        vector<RoomTrade> tradeRooms(roomCount);
        
        for (int i = 0; i < roomCount; i++) {
            cin >> tradeRooms[i].javaBeans >> tradeRooms[i].catFood;
            if (tradeRooms[i].catFood == 0) {
                tradeRooms[i].priceRatio = 1e9;
            } else {
                tradeRooms[i].priceRatio = static_cast<double>(tradeRooms[i].javaBeans) / tradeRooms[i].catFood;
            }
        }
        
        double maxBeans = getMaxJavaBeans(mouseCatFood, tradeRooms);
        
        cout << fixed << setprecision(3) << maxBeans << endl;
    }
    
    return 0;
}