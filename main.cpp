#include "geom.h"
#include "iohelper.h"

using namespace std;

const double VIEWING_ANGLE = 100.0;
const double F = 2.5;
const int INF = 1e9;
const int W = 260;
const int H = 66;

class Player{
public:
    Player() {}

    Player(const Point& p, const Vector& d, double a) 
        : position(p)
        , direction(d)
        , rotationAngle(a)
    {   }

    void Turn(bool left) {
        int sign = 1;
        if (!left) sign *= -1;
        direction.Rotate(DegreesToRadians(rotationAngle*sign));
    }

    void Move(bool forward) {
        int sign = 1;
        if (!forward) sign *= -1;
        position.x += direction.x * sign;
        position.y += direction.y * sign;
    }

    Point position;
    Vector direction;
    double rotationAngle;
};

int CalcHeight(double len) {
    return min(int(H / (len / F)), H);
}

void RayCasting(const Player& player, const vector<vector<Point>>& field, ScreenManager& screenManager) {
    double rotationAngle = VIEWING_ANGLE / W;
    pair<int, int> lastWall = {-1, -1};
    Vector ray = player.direction;
    ray.Rotate(DegreesToRadians(VIEWING_ANGLE / 2));

    for (int col = 0; col < W; ++col) {
        Line rayLine(player.position, ray.Shift(player.position));
        pair<int, int> curWall = {-1, -1};
        double len = INF;

        for (int i = 0; i != field.size(); ++i) { // polygons
            int n = field[i].size();
            for (int j = 0; j != n; ++j) { // walls 
                auto intersectionPoint = Segment::Intersection({field[i][j], field[i][(j + 1) % n]}, rayLine);
                if (!intersectionPoint) continue;
                Vector v(player.position, *intersectionPoint);
                if (Vector::DotProduct(v, player.direction) < 0) continue;
                if (v.Len() < len) {
                    len = v.Len();
                    curWall = {i, j}; 
                }
            }
        }
        
        int h = CalcHeight(len);
        bool isEdge = (lastWall != make_pair(-1, -1) && lastWall != curWall);
        lastWall = curWall;
        screenManager.DrawColumn(h, col, isEdge);

        ray.Rotate(DegreesToRadians(-rotationAngle));
    }
}

int main() {
    IOInit(); 
    ScreenManager screenManager(W, H);
 
    vector<vector<Point>> field = {{{20, 20}, {50, 20}, {50, 50}, {40, 50}, {40, 70}, {50, 70}, {50, 75}, {20, 75}, {20, 70}, {35, 70}, {35, 50}, {20, 50}}, 
                                   {{20, 20}, {20, 30}, {30, 30}, {30, 20}},
                                   {{35, 35}, {35, 40}, {40, 40}, {40, 35}}}; // set of field's polygons

    Player player({45, 45}, {1, 0}, VIEWING_ANGLE / W * 9);

    screenManager.RenderFrame({player.position.x, player.position.y, player.direction.x, player.direction.y} /* Player Info */);

    while (1) {
        switch (GetKey()) {
            case 'a': player.Turn(true /* left */); break;
            case 'd': player.Turn(false /* left */); break;
            case 'w': player.Move(true /* forward */); break;
            case 's': player.Move(false /* forward */); break;
        }
       
        RayCasting(player, field, screenManager); 

        screenManager.RenderFrame({player.position.x, player.position.y, player.direction.x, player.direction.y} /* Player Info */);
    }


    return 0;
}

