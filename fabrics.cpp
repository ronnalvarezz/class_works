#include <iostream>
#include <cstddef>
#include <stdexcept>

const size_t planarSize = 5;
const int planarTypes[planarSize] = {1, 2, 3, 5, 6};

namespace voronova
{
  enum Color {RED, GREEN, BLUE};
  Color makeColor(char c)
  {
    switch (c)
    {
    case 'r': return Color::RED;
    case 'g': return Color::GREEN;
    case 'b': return Color::BLUE;
    default: throw std::logic_error("Incorrect color");
    }
  }
  class Planar;
  class Colored;
  class Point;
  class Vector;
  class ColoredPoint;
  class Brush;
  class Circle;
  class ColoredCircle;

  class Planar
  {
  public:
    virtual ~Planar() = default;

    virtual std::ostream& info(std::ostream& out) = 0;
    virtual int x() const = 0;
    virtual int y() const = 0;
    virtual int abs_sqr() const = 0;
  };

  class Colored
  {
  public:
    virtual ~Colored() = default;

    virtual Color getColor() const = 0;
    virtual void setColor(Color color) = 0;
  };

  class Point: public Planar
  {
  public:
    Point(int x, int y):
      x_(x),
      y_(y)
    {}
    virtual ~Point() = default;

    virtual std::ostream& info(std::ostream& out) override
    {
      return out << "Point";
    }

    virtual int x() const override
    {
      return x_;
    }
    virtual int y() const override
    {
      return y_;
    }
    virtual int abs_sqr() const override
    {
      return x_ * x_ + y_ * y_;
    }
  private:
    int x_, y_;
  };
  Planar* clonePoint(std::istream& in)
  {
    int x = 0, y = 0;
    if (in >> x >> y)
    {
      return new Point(x, y);
    }
    throw std::logic_error("Incorrect point");
  }

  class Vector: public Planar
  {
  public:
    Vector(Point a, Point b):
      a_(a),
      b_(b)
    {}
    virtual ~Vector() = default;

    virtual std::ostream& info(std::ostream& out) override
    {
      return out << "Vector";
    }

    virtual int x() const override
    {
      return b_.x() - a_.x();
    }
    virtual int y() const override
    {
      return b_.y() - a_.y();
    }
    virtual int abs_sqr() const override
    {
      return (x() * x() + y() * y());
    }
  private:
    Point a_, b_;
  };
  Planar* cloneVector(std::istream& in)
  {
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    if (in >> x1 >> y1 >> x2 >> y2)
    {
      return new Vector(Point(x1, y1), Point(x2, y2));
    }
    throw std::logic_error("Incorrect vector");
  }

  class ColoredPoint: public Planar, public Colored
  {
  public:
    ColoredPoint(Point p, Color color):
      p_(p),
      cl_(color)
    {}
    virtual ~ColoredPoint() = default;

    virtual std::ostream& info(std::ostream& out) override
    {
      return out << "Colored point";
    }

    virtual int x() const override
    {
      return p_.x();
    }
    virtual int y() const override
    {
      return p_.y();
    }
    virtual int abs_sqr() const override
    {
      return p_.abs_sqr();
    }
    virtual Color getColor() const override
    {
      return cl_;
    }
    virtual void setColor(Color color) override
    {
      cl_ = color;
    }
  private:
    Point p_;
    Color cl_;
  };
  ColoredPoint* cloneColoredPoint(std::istream& in)
  {
    int x = 0, y = 0;
    char color = 0;
    if (in >> x >> y >> color)
    {
      return new ColoredPoint(Point(x, y), makeColor(color));
    }
    throw std::logic_error("Incorrect colored point");
  }
  Planar* cloneColoredPointPlanar(std::istream& in)
  {
    return cloneColoredPoint(in);
  }
  Colored* cloneColoredPointColored(std::istream& in)
  {
    return cloneColoredPoint(in);
  }

  class Brush: public Colored
  {
  public:
    Brush(Color color):
      cl_(color)
    {}
    virtual ~Brush() = default;

    virtual Color getColor() const override
    {
      return cl_;
    }
    virtual void setColor(Color color) override
    {
      cl_ = color;
    }
  private:
    Color cl_;
  };
  Colored* cloneBrush(std::istream& in)
  {
    char color = 0;
    if (in >> color)
    {
      return new Brush(makeColor(color));
    }
    throw std::logic_error("Incorrect brush");
  }

  class Circle: public Planar
  {
  public:
    Circle(Point c, int r):
      c_(c),
      r_(r)
    {}
    virtual ~Circle() = default;

    virtual std::ostream& info(std::ostream& out) override
    {
      return out << "Circle";
    }

    virtual int x() const override
    {
      return c_.x();
    }
    virtual int y() const override
    {
      return c_.y();
    }
    virtual int abs_sqr() const override
    {
      return c_.abs_sqr();
    }

    double sqr() const
    {
      return 3.14 * r_ * r_;
    }
    void move(Point p)
    {
      c_ = p;
    }
    void resize(int r)
    {
      r_ = r;
    }
  private:
    Point c_;
    int r_;
  };
  Planar* cloneCircle(std::istream& in)
  {
    int x = 0, y = 0, r = 0;
    if (in >> x >> y >> r)
    {
      return new Circle(Point(x, y), r);
    }
    throw std::logic_error("Incorrect circle");
  }

  class ColoredCircle: public Circle, public Colored
  {
  public:
    ColoredCircle(Point c, int r, Color color):
      Circle(c, r),
      cl_(color)
    {}
    virtual ~ColoredCircle() = default;

    virtual std::ostream& info(std::ostream& out) override
    {
      return out << "Colored circle";
    }

    virtual Color getColor() const override
    {
      return cl_;
    }
    virtual void setColor(Color color) override
    {
      cl_ = color;
    }
  private:
    Color cl_;
  };
  ColoredCircle* cloneColoredCircle(std::istream& in)
  {
    int x = 0, y = 0, r = 0;
    char color = 0;
    if (in >> x >> y >> r >> color)
    {
      return new ColoredCircle(Point(x, y), r, makeColor(color));
    }
    throw std::logic_error("Incorrect colored circle");
  }
  Planar* cloneColoredCirclePlanar(std::istream& in)
  {
    return cloneColoredCircle(in);
  }
  Colored* cloneColoredCircleColored(std::istream& in)
  {
    return cloneColoredCircle(in);
  }

  Planar* fabricPlanar(std::istream& in)
  {
    int num = 0;
    in >> num;
    switch (num)
    {
    case 1: return clonePoint(in);
    case 2: return cloneVector(in);
    case 3: return cloneColoredPointPlanar(in);
    case 5: return cloneCircle(in);
    case 6: return cloneColoredCirclePlanar(in);
    default: throw std::logic_error("Incorrect planar");
    }
  }

  Colored* fabricColored(std::istream& in)
  {
    int num = 0;
    in >> num;
    switch (num)
    {
    case 3: return cloneColoredPointColored(in);
    case 4: return cloneBrush(in);
    case 6: return cloneColoredCircleColored(in);
    default: throw std::logic_error("Incorrect colored");
    }
  }
}

namespace fabric
{
  class AbstractFabricPlanar
  {
  public:
    virtual ~AbstractFabricPlanar() = default;

    virtual voronova::Planar* create(std::istream& in) = 0;
  };

  class FabricPoint: public AbstractFabricPlanar
  {
  public:
    virtual ~FabricPoint() = default;
    virtual voronova::Planar* create(std::istream& in) override
    {
      int x = 0, y = 0;
      if (in >> x >> y)
      {
        return new voronova::Point(x, y);
      }
      throw std::logic_error("Incorrect point");
    }
  };

  class FabricVector: public AbstractFabricPlanar
  {
  public:
    virtual ~FabricVector() = default;
    virtual voronova::Planar* create(std::istream& in) override
    {
      int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
      if (in >> x1 >> y1 >> x2 >> y2)
      {
        return new voronova::Vector(voronova::Point(x1, y1), voronova::Point(x2, y2));
      }
      throw std::logic_error("Incorrect vector");
    }
  };

  class FabricColoredPoint: public AbstractFabricPlanar
  {
  public:
    virtual ~FabricColoredPoint() = default;
    virtual voronova::Planar* create(std::istream& in) override
    {
      int x = 0, y = 0;
      char color = 0;
      if (in >> x >> y >> color)
      {
        return new voronova::ColoredPoint(voronova::Point(x, y), voronova::makeColor(color));
      }
      throw std::logic_error("Incorrect colored point");
    }
  };

  class FabricCircle: public AbstractFabricPlanar
  {
  public:
    virtual ~FabricCircle() = default;
    virtual voronova::Planar* create(std::istream& in) override
    {
      int x = 0, y = 0, r = 0;
      if (in >> x >> y >> r)
      {
        return new voronova::Circle(voronova::Point(x, y), r);
      }
      throw std::logic_error("Incorrect circle");
    }
  };

  class FabricColoredCircle: public AbstractFabricPlanar
  {
  public:
    virtual ~FabricColoredCircle() = default;
    virtual voronova::Planar* create(std::istream& in) override
    {
      int x = 0, y = 0, r = 0;
      char color = 0;
      if (in >> x >> y >> r >> color)
      {
        return new voronova::ColoredCircle(voronova::Point(x, y), r, voronova::makeColor(color));
      }
      throw std::logic_error("Incorrect colored circle");
    }
  };

  void clearFabricsPlanar(AbstractFabricPlanar** fabricsPlanar, size_t size)
  {
    for (size_t j = 0; j < size; j++)
    {
      delete fabricsPlanar[j];
    }
    delete[] fabricsPlanar;
  }

  AbstractFabricPlanar** makeFabricsPlanar()
  {
    AbstractFabricPlanar** fabricsPlanar = new AbstractFabricPlanar*[planarSize];
    size_t i = 0;
    try
    {
      fabricsPlanar[i] = new FabricPoint();
      i++;
      fabricsPlanar[i] = new FabricVector();
      i++;
      fabricsPlanar[i] = new FabricColoredPoint();
      i++;
      fabricsPlanar[i] = new FabricCircle();
      i++;
      fabricsPlanar[i] = new FabricColoredCircle();
      i++;
    }
    catch (const std::exception& e)
    {
      clearFabricsPlanar(fabricsPlanar, i);
      throw;
    }
    return fabricsPlanar;
  }
}

std::ostream& printAbsSqr(std::ostream& out, voronova::Planar* pl)
{
  return pl->info(out) << ' ' << pl->abs_sqr();
}

void clear(voronova::Planar* pls[], size_t size)
{
  for (size_t i = 0; i < size; i++)
  {
    delete pls[i];
  }
  delete[] pls;
}

int main()
{
  size_t cap = 0;
  std::cin >> cap;
  if (!std::cin)
  {
    std::cerr << "Incorrect size\n";
    return 1;
  }

  fabric::AbstractFabricPlanar** fabrics = nullptr;
  try
  {
    fabrics = fabric::makeFabricsPlanar();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  voronova::Planar** pls = nullptr;
  try
  {
    pls = new voronova::Planar*[cap];
  }
  catch (const std::exception& e)
  {
    fabric::clearFabricsPlanar(fabrics, planarSize);
    std::cerr << e.what() << '\n';
    return 1;
  }

  size_t size = 0;
  while (size < cap && std::cin)
  {
    int num = 0;
    std::cin >> num;
    try
    {
      for (size_t j = 0; j < planarSize; j++)
      {
        if (num == planarTypes[j])
        {
          pls[size] = fabrics[j]->create(std::cin);
          size++;
        }
      }
    }
    catch (const std::exception& e)
    {
      fabric::clearFabricsPlanar(fabrics, planarSize);
      clear(pls, size);
      std::cerr << e.what() << '\n';
      return 1;
    }
  }

  for (size_t i = 0; i < size; i++)
  {
    printAbsSqr(std::cout, pls[i]) << '\n';
  }

  fabric::clearFabricsPlanar(fabrics, planarSize);
  clear(pls, size);
}
