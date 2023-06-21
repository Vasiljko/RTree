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

    Region(const Region&) = default;
    Region& operator=(const Region&) = default;
    Region(Region&&) = default;
    Region& operator=(Region&&) = default;

    std::pair<Point, Point> get_coords() const {return std::pair<Point,Point>{upper_left, bottom_right};}
    Point get_upper_left() const {return upper_left;}
    Point get_bottom_right() const {return bottom_right;}
    
    long long calculate_area() const{
        return 1LL*(upper_left.first - bottom_right.first)*(upper_left.second - bottom_right.second);
    }

    static bool in_range(int value, int min, int max){ return (value >= min) && (value <= max); }

    static bool regions_intersect(Region A, Region B){
        bool intersect_x = in_range(A.upper_left.first, B.upper_left.first, B.bottom_right.first) ||
                        in_range(B.upper_left.first, A.upper_left.first, A.bottom_right.first);

        bool intersect_y = in_range(A.upper_left.second, B.upper_left.second, B.bottom_right.second) ||
                        in_range(B.upper_left.second, A.upper_left.second, A.bottom_right.second);

        return intersect_x && intersect_y;
    }

    bool intersect(Region r) const{
        return regions_intersect(*this, r);
    }
    
    static Region calculate_mbr(const Region& f, const Region& s){
        Region region = { Point{std::min(f.upper_left.first, s.upper_left.first), std::min(f.upper_left.second, s.upper_left.second)},
                              Point{std::max(f.bottom_right.first, s.bottom_right.first), std::max(f.bottom_right.second, s.bottom_right.second)} };
        return region;
    }

    long long calculate_increase(Region& r) const{
        long long area = calculate_area();
        
        Region new_region = calculate_mbr(*this, r);

        long long new_area = new_region.calculate_area();

        return new_area - area;
    }

    static long long calculate_intersection_area(Region& f, Region& s){
        long long area1 = f.calculate_area();
        long long area2 = s.calculate_area();

        int dist_x = std::min(f.bottom_right.first, s.bottom_right.first) - std::max(f.upper_left.first, s.upper_left.first);
        int dist_y = (std::min(f.bottom_right.second, s.bottom_right.second) - std::max(f.upper_left.second, s.upper_left.second));
        
        long long area_intersect = 0;
        if( dist_x > 0 && dist_y > 0 ){
            area_intersect = 1LL * dist_x * dist_y;
        }

        return area1 + area2 - area_intersect;
    }

    friend std::ostream& operator <<(std::ostream& os, const Region& r){
        os<<"[("<<r.upper_left.first<<","<<r.upper_left.second<<"), ("<<r.bottom_right.first<<","<<r.bottom_right.second<<")]";

        return os;
    }
};

#endif