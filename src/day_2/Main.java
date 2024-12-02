package day_2;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.util.function.Predicate;
import java.util.stream.Stream;

public class Main {
    public static void main(String... args) throws IOException {
        List<List<Integer>> data = readData("src/day_2/data.csv");

        // part 1: count number sequences homogeneously increasing or decreasing, by steps of min 1 and max 3
        long validSequences = data.stream().filter(getStrictSequenceValidator()).count();
        System.out.println(validSequences);

        // part 2: same as part 1, but allowing one deviating data point
        long validSequences2 = data.stream().filter(getStrictSequenceValidator().or(getSubListValidator())).count();
        System.out.println(validSequences2);
    }

    public static List<List<Integer>> readData(String filename) throws IOException {
        try (Stream<String> lines = Files.lines(Path.of(filename))) {
            return lines.map(x -> Arrays.stream(x.split(";")).map(Integer::parseInt).toList()).toList();
        }
    }

    public static List<List<Integer>> createSubLists(List<Integer> seq) {
        var y = Stream.iterate(0, x -> x + 1).limit(seq.size()).map(index -> {
            List<Integer> clone = new ArrayList<>(seq);
            clone.remove((int) index);
            return clone;
        }).toList();
        return y;
    }

    public static Predicate<List<Integer>> getSubListValidator() {
        return seq -> createSubLists(seq).stream().filter(getStrictSequenceValidator()).findFirst().isPresent();
    }

    public static Predicate<List<Integer>> getStrictSequenceValidator() {
        return seq -> {
            Iterator<Integer> it = seq.stream().skip(1).iterator();
            boolean increasing = seq.get(1) > seq.get(0);
            boolean valid = seq.stream()
                .filter(x -> it.hasNext())
                .filter(getNumberValidator(increasing, it))
                .findFirst()
                .isEmpty();
            return valid;
        };
    }

    public static Predicate<Integer> getNumberValidator(boolean increasing, Iterator<Integer> nextNumbers) {
        return cur -> {
            int next = nextNumbers.next();
            boolean invalid = Math.abs(cur - next) > 3 || cur == next || next < cur && increasing || next > cur && !increasing;
            return invalid;
        };
    }
}
