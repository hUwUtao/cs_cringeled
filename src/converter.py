from mido import MidiFile


def midi_to_pitch(note):
    return int(440 * (2 ** ((note - 69) / 12)))


notes = list(
    map(
        lambda a: a[1],
        filter(
            lambda a: not (a[0] & 0x1) and a[1].type == "note_on",
            enumerate(MidiFile("../assets/stolen.mid", clip=True).tracks[0]),
        ),
    )
)

pitches = list(map(lambda n: midi_to_pitch(n.note), notes))
rest = list(map(lambda n: n.time, notes))
rest[-1] = 2048
print(
    """
#define NOTES_COUNT {}

unsigned int notes[] = {};

unsigned rest[] = {};
""".format(
        len(notes),
        "{" + (", ".join(map(str, pitches))) + "}",
        "{" + (", ".join(map(str, rest))) + "}",
    )
)
# print(mid)
