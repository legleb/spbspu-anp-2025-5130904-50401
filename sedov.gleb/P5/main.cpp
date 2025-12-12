#include <iostream>
#include <cstddef>

namespace sedov
{
  struct point_t
  {
    double x, y;
  }

  struct rectangle_t
  {
    double width, height;
    point_t pos;
  }

  struct Shape
  {
    virtual ~Shape() = default;
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(point_t p) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double k) = 0;
  };

  struct Rectangle: Shape
  {
    Rectangle(double w, double h, point_t p);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t p) override;
    void move(double dx, double dy) override;
    void scale(double k) override;
  private:
    double width_, height_;
    point_t center_;
  };

  void scaleByPoint(Shape ** fs, size_t s, point_t p, double k);
  double getAllArea(Shape ** p, size_t s);
  rectangle_t getAllFrameRect(Shape ** p, size_t s);
  void output(Shape ** fs, size_t s);
}

int main()
{}

sedov::Rectangle::Rectangle(double w, double h, point_t p):
  Shape(),
  width_(w),
  height_(h),
  center_(p)
{
  if (w <= 0 || h <= 0)
  {
    throw std::invalid_argument("Invalid size");
  }
}

double sedov::Rectangle::getArea() const
{
  return width_ * height_;
}

sedov::rectangle_t sedov::Rectangle::getFrameRect() const
{
  rectangle_t frameRect;
  frameRect.width = width_;
  frameRect.height = height_;
  frameRect.pos = center_;
  return frameRect;
}

void sedov::Rectangle::move(point_t p)
{
  center_ = p;
}

void sedov::Rectangle::move(double dx, double dy)
{
  center_.x += dx;
  center_.y += dy;
}

void sedov::Rectangle::scale(double k)
{
  width_ *= k;
  height_ *= k;
}

void sedov::scaleByPoint(Shape ** fs, size_t s, point_t p, double k)
{
  if (!s || fs == nullptr)
  {
    throw std::invalid_argument("Empty array of figures");
  }
  for (size_t i = 0; i < s; ++i)
  {
    point_t a = fs[i]->getFrameRect().pos;
    double dx = (a.x - p.x) * (k - 1);
    double dy = (a.y - p.y) * (k - 1);
    fs[i]->move(dx, dy);
    fs[i]->scale(k);
  }
}

double sedov::getAllArea(Shape ** p, size_t s)
{
  if (!s || p == nullptr)
  {
    throw std::invalid_arguments("Empty array of figures");
  }
  double area = 0.0;
  for (size_t i = 0; i < s; ++i)
  {
    area += p[i]->getArea();
  }
  return area;
}

sedov::rectangle_t sedov::getAllFrameRect(Shape ** p, size_t s)
{
  if (!s || p == nullptr)
  {
    throw std::invalid_argument("Empty array of figures");
  }
  rectangle_t allFrameRect;
  rectangle_t f = p[0]->getFrameRect();
  double minX = f.pos.x - f.width / 2;
  double maxX = f.pos.x + f.width / 2;
  double minY = f.pos.y - f.height / 2;
  double maxY = f.pos.y + f.height / 2;
  for (size_t i = 1; i < s; ++i)
  {
    f = p[i]->getFrameRect();
    minX = std::min(minX, f.pos.x - f.width / 2);
    maxX = std::max(maxX, f.pos.x + f.width / 2);
    minY = std::min(minY, f.pos.y - f.height / 2);
    maxY = std::max(maxY, f.pos.y + f.height / 2);
  }
  allFrameRect.width = maxX - minX;
  allFrameRect.height = maxY - minY;
  allFrameRect.pos = {(minX + maxX) / 2, (minY + maxY) / 2};
  return allFrameRect;
}

void sedov::output(Shape ** fs, size_t s)
{
  for (size_t i = 0; i < s; ++i) {
    rectangle_t a = fs[i]->getFrameRect();
    std::cout << "Figure " << i << ":\n";
    std::cout << "\tArea: " << fs[i]->getArea() << "\n";
    std::cout << "\tFrame rectangle:\n";
    std::cout << "\t\tWidth: " << a.width << "\n";
    std::cout << "\t\tHeight: " << a.height << "\n";
    std::cout << "\t\tCenter: x = " << a.pos.x << " y = " << a.pos.y << "\n";
  }
  std::cout << "SumArea: " << getAllArea(fs, s) << "\n";
  rectangle_t b = getAllFrameRect(fs, s);
  std::cout << "Generic frame:\n";
  std::cout << "\tWidth: " << b.width << "\n";
  std::cout << "\tHeight: " << b.height << "\n";
  std::cout << "\tCenter: x = " << b.pos.x << " y = " << b.pos.y << "\n";
}
