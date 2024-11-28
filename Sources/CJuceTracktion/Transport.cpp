#include "./include/Transport.h"


Transport::Transport(te::TransportControl& t) : transport(t) {}

void Transport::play(bool shouldPlay) {
    transport.play(shouldPlay);
}

void Transport::stop(bool discardRecordings, bool rewindToStart) {
    transport.stop(discardRecordings, rewindToStart);
}

double Transport::getCurrentPosition() const {
    return transport.getPosition().inSeconds();
}

void Transport::setPosition(double newPosition) {
    transport.setPosition(te::TimePosition::fromSeconds(newPosition));
} 