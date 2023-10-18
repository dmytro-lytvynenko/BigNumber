#include "big_number.h"
#include <iostream>
#include <exception>

void testEquality(const std::string& testName, const std::string& expected, const std::string& actual) {
    std::cout << testName << ":" << std::endl;
    std::cout << "Expected: " << expected << std::endl;
    std::cout << "Actual:   " << actual << std::endl;

    if (expected == actual) {
        std::cout << testName << " Passed!" << std::endl;
    } else {
        std::cout << testName << " Failed!" << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    try {
        std::string originalHex = "51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4";
        BigNumber firstNum;
        firstNum.setHex(originalHex);
        BigNumber secondNum;
        secondNum.setHex("403db8ad88a3932a0b7e8189aed9eeffb8121dfac05c3512fdb396dd73f6331c");

        std::string resultHex = firstNum.getHex();
        testEquality("Initialization Test", originalHex, resultHex);

        BigNumber xorResult = firstNum ^ secondNum;
        testEquality("XOR Test", "1182d8299c0ec40ca8bf3f49362e95e4ecedaf82bfd167988972412095b13db8", xorResult.getHex());

        BigNumber andResult = firstNum & secondNum;
        testEquality("AND Test", "403d208400a113220340808088d16a1b10121078400c1002748196dd62460204", andResult.getHex());

        BigNumber orResult = firstNum | secondNum;
        testEquality("OR Test", "51bff8ad9cafd72eabffbfc9befffffffcffbffaffdd779afdf3d7fdf7f73fbc", orResult.getHex());

        BigNumber invResult = ~firstNum;
        testEquality("INV Test", "ae409f7beb52a8d95c3e413f670884e4ab004d878072ad758b3e280219b8f15b", invResult.getHex());

        BigNumber shiftedRight = firstNum;
        shiftedRight >>= 4;
        testEquality("Shift Right Test", "51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea", shiftedRight.getHex());

        BigNumber shiftedLeft = firstNum;
        shiftedLeft <<= 4;
        testEquality("Shift Left Test", "51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea40", shiftedLeft.getHex());

        BigNumber a("36f028580bb02cc8272a9a020f4200e346e276ae664e45ee80745574e2f5ab80");
        BigNumber b("70983d692f648185febe6d6fa607630ae68649f7e6fc45b94680096c06e4fadb");
        BigNumber sumResult = a + b;
        testEquality("Sum Test", "a78865c13b14ae4e25e90771b54963ee2d68c0a64d4a8ba7c6f45ee0e9daa65b", sumResult.getHex());

        BigNumber c("33ced2c76b26cae94e162c4c0d2c0ff7c13094b0185a3c122e732d5ba77efebc");
        BigNumber d("22e962951cb6cd2ce279ab0e2095825c141d48ef3ca9dabf253e38760b57fe03");
        BigNumber subtractResult = c - d;
        testEquality("Subtraction Test", "10e570324e6ffdbc6b9c813dec968d9bad134bc0dbb061530934f4e59c2700b9", subtractResult.getHex());

    } catch (const std::exception& e) {
        std::cout << "An error occurred during initialization or operation: " << e.what() << std::endl;
    }

    return 0;
}
