/**
 * @brief Red Pitaya simple version strings. To be embedded in binaries
 * at build time for SW traceability.
 * @author @ClemtoClem (https://github.com/ClemtoClem/RedPitaya)
 */

#ifndef VERSION_HPP
#define VERSION_HPP

#include <string>

struct Version {
    int major;
    int minor;
    int patch;
    int build;

    Version() : major(0), minor(0), patch(0), build(0) {}

    Version(int major, int minor, int patch, int build) : major(major), minor(minor), patch(patch), build(build) {}

    std::string toString() {
        std::stringstream ss;
        ss << major << "." << minor << "." << patch << "." << build;
        return ss.str();
    }

    bool operator==(const Version &other) const {
        return major == other.major && minor == other.minor && patch == other.patch && build == other.build;
    }

    bool operator>(const Version &other) const {
        if (major > other.major) return true;
        if (major < other.major) return false;
        if (minor > other.minor) return true;
        if (minor < other.minor) return false;
        if (patch > other.patch) return true;
        if (patch < other.patch) return false;
        if (build > other.build) return true;
        if (build < other.build) return false;
        return false;
    }
};

#endif /* VERSION_HPP */