#ifndef REGION_H
#define REGION_H

#include <utility>
#include <iostream>
#include <algorithm>

class Region
{
    using Point = std::pair<int, int>;
    Point upper_left, bottom_right;

public:
    Region() = default;
    Region(const Point &a, const Point &b) : upper_left(a), bottom_right(b) {}

    std::pair<Point, Point> get_coords() const {return std::pair<Point,Point>{upper_left, bottom_right};}
    Point get_upper_left() const {return upper_left;}
    Point get_bottom_right() const {return bottom_right;}
    
    long long calculate_area() const{
        return 1LL*(upper_left.first - bottom_right.first)*(upper_left.second - bottom_right.second);
    }

    bool intersect(Region& r) const{
        if (upper_left.first == bottom_right.first || upper_left.second == bottom_right.second || r.bottom_right.first == r.upper_left.first || r.upper_left.second == r.bottom_right.second)
            return false;
   
        if (upper_left.first > r.bottom_right.first || r.upper_left.first > bottom_right.first)
            return false;
    
        if (bottom_right.second > r.upper_left.second || r.bottom_right.second > upper_left.second)
            return false;
    
        return true;
    }
    
    long long calculate_increase(Region& r) const{
        long long area = calculate_area();
        Region new_region = { Point{std::min(upper_left.first, r.upper_left.first), std::min(upper_left.second, r.upper_left.second)},
                              Point{std::max(bottom_right.first, r.bottom_right.first), std::max(bottom_right.second, r.bottom_right.second)} };

        long long new_area = new_region.calculate_area();

        return new_area - area;
    }

    friend std::ostream& operator <<(std::ostream& os, const Region& r){
        os<<"("<<r.upper_left.first<<","<<r.upper_left.second<<"), ("<<r.bottom_right.first<<","<<r.bottom_right.second<<")\n";

        return os;
    }
};

#endif
/*
if(!region.intersect(entries[i].second))continue;
        
        int increase = entries[i].second.calculate_increase(region);

        if(increase < least_increase){
            least_increase = increase;
            id = i;
        }else if(increase == least_increase){
            id = entries[id].calculate_area() < entries[i].calculate_area() ? id : i;
        }
*/