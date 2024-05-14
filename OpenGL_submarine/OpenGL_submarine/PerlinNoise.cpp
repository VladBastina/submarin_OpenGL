#include <cmath>
#include <vector>
#include <algorithm>
#include <random>

class PerlinNoise {
private:
    std::vector<int> permutation;

    double fade(double t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }
    double lerp(double t, double a, double b)
    {
        return a + t * (b - a);
    }
    double grad(int hash, double x, double y, double z)
    {
        int h = hash & 15;
        double u = h < 8 ? x : y;
        double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

public:
    double noise(double x, double y, double z)
    {
        int X = (int)floor(x) & 255;
        int Y = (int)floor(y) & 255;
        int Z = (int)floor(z) & 255;

        int X1;
        if (X == 255)
        {
            X1 = 0;
        }
        else
        {
            X1 = X + 1;
        }

        x -= floor(x);
        y -= floor(y);
        z -= floor(z);

        double u = fade(x);
        double v = fade(y);
        double w = fade(z);

        int A = (permutation[X] + Y)%255;
        int AA = (permutation[A] + Z)%255;
        int AB = (permutation[A + 1] + Z)%255;
        int B = (permutation[X1] + Y)%255;
        int BA = (permutation[B] + Z)%255;
        int BB = (permutation[B + 1] + Z)%255;

        int AA1, BA1, AB1, BB1;

        if ((AA + 1) == 256)
        {
            AA1 = 0;
        }
        else
        {
            AA1 = AA + 1;
        }

        if ((BA + 1) == 256)
        {
            BA1 = 0;
        }
        else
        {
            BA1 = BA + 1;
        }

        if ((AB + 1) == 256)
        {
            AB1 = 0;
        }
        else
        {
            AB1 = AB + 1;
        }

        if ((BB + 1) == 256)
        {
            BB1 = 0;
        }
        else
        {
            BB1 = BB + 1;
        }

        return lerp(w, lerp(v, lerp(u, grad(permutation[AA], x, y, z),
            grad(permutation[BA], x - 1, y, z)),
            lerp(u, grad(permutation[AB], x, y - 1, z),
                grad(permutation[BB], x - 1, y - 1, z))),
            lerp(v, lerp(u, grad(permutation[AA1], x, y, z - 1),
                grad(permutation[BA1], x - 1, y, z - 1)),
                lerp(u, grad(permutation[AB1], x, y - 1, z - 1),
                    grad(permutation[BB1], x - 1, y - 1, z - 1))));
    }

    PerlinNoise()
    {
        for (int i = 0; i < 256; i++)
        {
            permutation.push_back(i);
        }
        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(permutation.begin(), permutation.end(), g);
    }
};



