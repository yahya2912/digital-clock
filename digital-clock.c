#include <stdio.h>
#include <time.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>

#define WIDTH  1920
#define HEIGHT 1080

float segment_width     = 200.0f;
float segment_thickness =  50.0f;
float segment_gap       =  30.0f;

#define COLOR_ACTIVE   (Color){ 220,  30,  30, 255 }   // red
#define COLOR_INACTIVE (Color){  40,  40,  40, 255 }   // dark grey

const uint8_t SEGMENTS[10] = {
    0b1110111, // 0: a b c e f g
    0b0010010, // 1: c f
    0b1011101, // 2: a c d e g
    0b1011011, // 3: a c d f g
    0b0111010, // 4: b c d f
    0b1101011, // 5: a b d f g
    0b1101111, // 6: a b d e f g
    0b1010010, // 7: a c f
    0b1111111, // 8: all
    0b1111011 // 9: a b c d f g
};

void Draw_Segment(Vector2 center, bool horizontal, Color color)
{
    float half = segment_width / 2.0f - segment_gap;

    Vector2 a, b, c, d, e, f;
    if (horizontal) {
        a = (Vector2){ center.x - half - segment_thickness / 2.0f, center.y };
        b = (Vector2){ center.x - half,  center.y + segment_thickness / 2.0f };
        c = (Vector2){ center.x - half,  center.y - segment_thickness / 2.0f };
        d = (Vector2){ center.x + half,  center.y + segment_thickness / 2.0f };
        e = (Vector2){ center.x + half,  center.y - segment_thickness / 2.0f };
        f = (Vector2){ center.x + half + segment_thickness / 2.0f, center.y };
    } else {
        a = (Vector2){ center.x,                             center.y - half - segment_thickness / 2.0f };
        b = (Vector2){ center.x - segment_thickness / 2.0f, center.y - half };
        c = (Vector2){ center.x + segment_thickness / 2.0f, center.y - half };
        d = (Vector2){ center.x - segment_thickness / 2.0f, center.y + half };
        e = (Vector2){ center.x + segment_thickness / 2.0f, center.y + half };
        f = (Vector2){ center.x,                             center.y + half + segment_thickness / 2.0f };
    }
    Vector2 points[] = { a, b, c, d, e, f };
    DrawTriangleStrip(points, 6, color);
}

void Draw_Digit(Vector2 center, int digit)
{
    if (digit < 0 || digit > 9) return;
    uint8_t mask = SEGMENTS[digit];

    struct { Vector2 pos; bool horiz; } segs[7] = {
        { { center.x,                        center.y - segment_width           }, true  }, // a top
        { { center.x - segment_width / 2.0f, center.y - segment_width / 2.0f   }, false }, // b top-left
        { { center.x + segment_width / 2.0f, center.y - segment_width / 2.0f   }, false }, // c top-right
        { { center.x,                        center.y                           }, true  }, // d middle
        { { center.x - segment_width / 2.0f, center.y + segment_width / 2.0f   }, false }, // e bot-left
        { { center.x + segment_width / 2.0f, center.y + segment_width / 2.0f   }, false }, // f bot-right
        { { center.x,                        center.y + segment_width           }, true  }, // g bottom
    };

    for (int i = 0; i < 7; i++) {
        bool active = (mask >> (6 - i)) & 1;
        Color color = active ? COLOR_ACTIVE : COLOR_INACTIVE;
        Draw_Segment(segs[i].pos, segs[i].horiz, color);
    }
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Digital Clock");
    SetTargetFPS(30);

    while (!WindowShouldClose())
    {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        int h1 = t->tm_hour / 10;
        int h2 = t->tm_hour % 10;
        int m1 = t->tm_min  / 10;
        int m2 = t->tm_min  % 10;
        int s1 = t->tm_sec  / 10;
        int s2 = t->tm_sec  % 10;

        BeginDrawing();
        ClearBackground(BLACK);

        Draw_Digit((Vector2){ WIDTH/2 - 800.0f, HEIGHT/2 }, h1);
        Draw_Digit((Vector2){ WIDTH/2 - 500.0f, HEIGHT/2 }, h2);

        DrawCircle(WIDTH/2 - 325.0f, HEIGHT/2 - 45.0f, 25.0f, COLOR_ACTIVE);
        DrawCircle(WIDTH/2 - 325.0f, HEIGHT/2 + 45.0f, 25.0f, COLOR_ACTIVE);

        Draw_Digit((Vector2){ WIDTH/2 - 150.0f, HEIGHT/2 }, m1);
        Draw_Digit((Vector2){ WIDTH/2 + 150.0f, HEIGHT/2 }, m2);

        DrawCircle(WIDTH/2 + 325.0f, HEIGHT/2 - 45.0f, 25.0f, COLOR_ACTIVE);
        DrawCircle(WIDTH/2 + 325.0f, HEIGHT/2 + 45.0f, 25.0f, COLOR_ACTIVE);

        Draw_Digit((Vector2){ WIDTH/2 + 500.0f, HEIGHT/2 }, s1);
        Draw_Digit((Vector2){ WIDTH/2 + 800.0f, HEIGHT/2 }, s2);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
