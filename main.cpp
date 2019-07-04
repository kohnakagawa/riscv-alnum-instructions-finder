#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include "disasm.h"

constexpr auto alnum = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
constexpr auto num_alnum = strlen(alnum);

constexpr auto ascii = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
constexpr auto num_ascii = strlen(ascii);

using cnt_type = std::remove_const<decltype(num_alnum)>::type;

void print_usage(const char* prog_name) {
    std::cout << "Usage:\n";
    std::cout << prog_name << " [32/64] [alnum/ascii]";
    std::cout << std::endl;
}

int main(const int argc, const char* argv[]) {
    if (argc != 3) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const std::string bits(argv[1]);
    if (bits != "32" && bits != "64") {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const std::string string_range(argv[2]);
    if (string_range != "alnum" && string_range != "ascii") {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const auto xlen = std::stoi(bits);
    disassembler_t disas(xlen);

    const char* target_str = nullptr;
    cnt_type cnt = 0;
    if (string_range == "alnum") {
        target_str = alnum;
        cnt = num_alnum;
    } else {
        target_str = ascii;
        cnt = num_ascii;
    }

    std::cout << std::hex;
    for (cnt_type i = 0; i < cnt; i++) {
        if ((target_str[i] & 0b11) == 0b11) continue;
        for (cnt_type j = 0; j < cnt; j++) {
            const insn_bits_t inst_code = (target_str[j] << 8) | target_str[i];
            const auto inst_str = disas.disassemble(inst_code);
            if (inst_str != "unknown") {
                std::cout << inst_str << std::endl;
            }
        }
    }

    for (cnt_type i = 0; i < cnt; i++) {
        if ((target_str[i] & 0b11100) == 0b11100) continue;
        if ((target_str[i] & 0b11) != 0b11) continue;
        for (cnt_type j = 0; j < cnt; j++) {
            for (cnt_type k = 0; k < cnt; k++) {
                for (cnt_type l = 0; l < cnt; l++) {
                    const insn_bits_t inst_code =
                        (target_str[l] << 24) | (target_str[k] << 16) | (target_str[j] << 8) | target_str[i];
                    const auto inst_str = disas.disassemble(inst_code);
                    if (inst_str != "unknown") {
                        std::cout << inst_str << std::endl;
                    }
                }
            }
        }
    }

    return EXIT_SUCCESS;
}
