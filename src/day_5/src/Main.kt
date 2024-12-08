import java.io.File

fun main() {
    val lines = File("data.txt").readLines()
    val forwardConstraints: Map<String, Set<String>> = readConstraints(lines)
    val backwardConstraints: Map<String, Set<String>> = readConstraints(lines, false)
    val updates: List<List<String>> = readUpdates(lines)

    // part 1: find updates matching constraints, then sum their middle pages
    val validUpdates = updates.filter{
        it.mapIndexed { i, page ->
            it.take(i).none { prev -> forwardConstraints.getOrDefault(page, setOf()).contains(prev) }
            && it.drop(i + 1).none { next -> backwardConstraints.getOrDefault(page, setOf()).contains(next) }
        }.all { x -> x }
    }
    val sumOfValidMiddlePages = validUpdates.map { it[it.size / 2] }.map(String::toInt).sumOf { it }
    println(sumOfValidMiddlePages)

    // part 2: fix invalid updates to match constraints, then sum their new middle pages
    val fixedUpdates = updates.filter { !validUpdates.contains(it) }.map {
        var sorted = it
        while(true) {
            for(i in 1..it.size) {
                val page = sorted[i]
                val invalidPrevs = sorted.take(i).filter { prev -> forwardConstraints.getOrDefault(page, setOf()).contains(prev) }
                val validPrevs = sorted.take(i).filterNot(invalidPrevs::contains)
                val invalidNexts = sorted.drop(i + 1).filter { next -> backwardConstraints.getOrDefault(page, setOf()).contains(next) }
                val validNexts = sorted.drop(i + 1).filterNot(invalidNexts::contains)
                if (invalidPrevs.isEmpty() && invalidNexts.isEmpty()) continue
                sorted = validPrevs + invalidNexts + listOf(page) + invalidPrevs + validNexts
                break
            }
            val valid = sorted.mapIndexed { i, page ->
                sorted.take(i).none { prev -> forwardConstraints.getOrDefault(page, setOf()).contains(prev) }
                && sorted.drop(i + 1).none { next -> backwardConstraints.getOrDefault(page, setOf()).contains(next) }
            }.all { x -> x }
            if (valid) break
        }
        sorted
    }
    val sumOfFixedMiddlePages = fixedUpdates.map { it[it.size / 2] }.map(String::toInt).sumOf { it }
    println(sumOfFixedMiddlePages)
}

fun readConstraints(lines: List<String>, forward: Boolean = true): Map<String, Set<String>> {
    val constraints: MutableMap<String, MutableSet<String>> = mutableMapOf()
    lines.filter{ it.contains('|') }.forEach {
        val (first, second) = if (forward) it.split("|") else it.split("|").reversed()
        if (!constraints.contains(first)) {
            constraints[first] = mutableSetOf(second)
        } else {
            constraints[first]!!.add(second)
        }
    }
    return constraints
}

fun readUpdates(lines: List<String>): List<List<String>> {
    return lines
        .filter { it.contains(",") }
        .map { it.split(",") }
}
