#include <ncurses.h>
#include <unordered_set>
#include <deque>

const int ROWS = 20;
const int COLUMNS = 60;

struct Point {
  int x;
  int y;
  bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
  }
};

namespace std {
  template <>
  struct hash<Point> {
    std::size_t operator()(const Point& p) const {
      return std::hash<int>()(p.x) ^ std::hash<int>()(p.y);
    }
  };
}

int main() {
  initscr();
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);
  curs_set(0);

  std::unordered_set<Point> snake_points;
  std::deque<Point> snake;
  snake.push_back({COLUMNS / 2, ROWS / 2});
  snake_points.insert(snake.front());

  Point food = {-1, -1};
  while (true) {
    clear();

    // Generate food if it doesn't exist
    if (food.x == -1 || food.y == -1) {
      do {
        food = {rand() % COLUMNS, rand() % ROWS};
      } while (snake_points.count(food) > 0);
    }

    // Draw food
    mvaddch(food.y, food.x, 'O');

    // Draw snake
    for (const Point& p : snake) {
      mvaddch(p.y, p.x, '#');
    }

    refresh();

    // Get user input
    int ch = getch();
    int dx = 0;
    int dy = 0;
    if (ch == KEY_LEFT) {
      dx = -1;
    } else if (ch == KEY_RIGHT) {
      dx = 1;
    } else if (ch == KEY_UP) {
      dy = -1;
    } else if (ch == KEY_DOWN) {
      dy = 1;
    }

    // Update snake position
    Point head = snake.front();
    Point new_head = {head.x + dx, head.y + dy};
    snake.push_front(new_head);
    snake_points.insert(new_head);

    // Check if snake has hit a wall or itself
    if (new_head.x < 0 || new_head.x >= COLUMNS || new_head.y < 0 || new_head.y >= ROWS || snake_points.count(new_head) > 1) {
      break;
    }

    // Check if snake has eaten food
    if (new_head == food) {
      food = {-1, -1};
    } else {
      Point tail = snake.back();
      snake.pop_back();
      snake_points.erase(tail);
    }
  }

  endwin();
