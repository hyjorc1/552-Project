#include <iostream>

/* A simple class used to represent a person's weight.
   This ensures that the weight cannot be below zero */
class Weight
{
public:
    Weight(float w=0);

    float value();

    Weight operator+(Weight other);
    Weight operator-(Weight other);

private:
    float _weight;
};

Weight::Weight(float w)
{
    if (w > 0)
    {
        this->_weight = w;
    }
    else
    {
        this->_weight = 0;
    }
}

float Weight::value()
{
    return this->_weight;   
}

Weight Weight::operator+(Weight other)
{
    return Weight( this->_weight + other._weight );
}

Weight Weight::operator-(Weight other)
{
    return Weight( this->_weight - other._weight );
}

int main()
{
    Weight w;

    std::cout << "default " << w.value() << std::endl;

    w = w + Weight(5.0);

    std::cout << "add 5.0 " << w.value() << std::endl;

    for (int i=0; i<6; ++i)
    {
        w = w - Weight(1.0);
        std::cout << "subtract 1.0 " << w.value() << std::endl;
    }

    return 0;
}