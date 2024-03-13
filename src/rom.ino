#define u64 unsigned long
#define u16 unsigned short int
#define u8 unsigned char

#define LEDS 5
u8 pins[] = {2, 4, 6, 8, 12};
#define SPEAKER_PIN 13
#define GLOBAL_REST_THRESHOLD .8
// to make music "thread" tick slower, but music thread is constant afterall :))
#define MSCHEDULE_UNSYNC 3

// states
bool led_phase = 1;
u8 note_tick = 0;
u64 tick = 0; // global clock

// chart data

#define NOTES_COUNT 98

u16 notes[] = {587, 493, 493, 523, 587, 587, 659, 391, 391, 440, 493, 440, 369, 369, 391, 440, 440, 440, 391, 440, 493, 293, 391, 440, 493, 493, 493, 440, 493, 587, 440, 440, 369, 369, 391, 440, 293, 440, 440, 391, 587, 587, 493, 587, 493, 587, 659, 587, 659, 587, 440, 493, 493, 493, 440, 391, 391, 493, 587, 329, 391, 440, 440, 391, 440, 493, 587, 493, 587, 659, 587, 493, 493, 440, 391, 440, 493, 391, 440, 493, 440, 440, 391, 329, 391, 440, 440, 440, 391, 329, 293, 293, 440, 440, 391, 329, 293, 391};

u16 rest[] = {240, 455, 227, 227, 455, 227, 227, 455, 227, 227, 683, 227, 455, 227, 227, 455, 455, 227, 227, 227, 227, 911, 455, 455, 683, 227, 227, 227, 227, 227, 683, 227, 455, 227, 227, 455, 455, 455, 455, 1367, 455, 455, 2279, 455, 455, 455, 2279, 227, 227, 455, 455, 683, 227, 227, 227, 455, 455, 227, 227, 455, 455, 683, 227, 455, 455, 455, 455, 227, 227, 455, 2279, 455, 227, 227, 2279, 227, 227, 455, 455, 2279, 455, 227, 227, 227, 227, 683, 227, 227, 227, 227, 227, 1367, 455, 683, 227, 455, 455, 2048};

// actual prog

void setup()
{
    pinMode(13, OUTPUT);
    for (u8 i = 0; i < LEDS; i++)
        pinMode(pins[i], OUTPUT);
}

void led_thread()
{
    for (u8 i = 0; i < LEDS; i++)
    {
        u8 counter = led_phase ? pins[i] : pins[LEDS - i];
        if (i == tick % LEDS)
            digitalWrite(counter, HIGH);
        else
            digitalWrite(counter, LOW);
    }

    if (led_phase ? (tick % LEDS) == LEDS - 1 : (LEDS - 1 - (tick % LEDS)) == 0)
        led_phase = !led_phase;
}

void note_thread()
{
    // loop clock?
#define NOTE_TICK thread_tick % NOTES_COUNT

    u64 thread_tick = tick / MSCHEDULE_UNSYNC;
    if ((tick % MSCHEDULE_UNSYNC) == 0)
        // 64 bit copy wth?!
        tone(SPEAKER_PIN, notes[NOTE_TICK], rest[NOTE_TICK] * GLOBAL_REST_THRESHOLD * .7);
    delay(rest[NOTE_TICK] * GLOBAL_REST_THRESHOLD / MSCHEDULE_UNSYNC);
}

void loop()
{
    // schedulers, by default every thread should un-sync itself
    led_thread();
    note_thread(); // blocking

    tick++;
}