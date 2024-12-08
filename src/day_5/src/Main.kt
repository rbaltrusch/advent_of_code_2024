import java.io.File

fun main() {
    val lines = File("data.txt").readLines()
    val forwardConstraints: Map<String, Set<String>> = readConstraints(lines)
    val backwardConstraints: Map<String, Set<String>> = readConstraints(lines, false)
    val updates: List<List<String>> = readUpdates(lines)

    // part 1: find updates matching constraints, then sum their middle pages
    val forwardInvalidator = getConstraintValidator(forwardConstraints)
    val backwardInvalidator = getConstraintValidator(backwardConstraints)
    val validator = getValidator(forwardInvalidator, backwardInvalidator)
    val validUpdates = updates.filter(validator)
    val sumOfValidMiddlePages = validUpdates.map { it[it.size / 2] }.map(String::toInt).sumOf { it }
    println(sumOfValidMiddlePages)

    // part 2: fix invalid updates to match constraints, then sum their new middle pages
    val fixedUpdates = updates.filter { !validUpdates.contains(it) }.map {
        var sorted = it
        while(true) {
            for(i in 1..it.size) {
                val page = sorted[i]
                val invalidPrevs = sorted.take(i).filter(forwardInvalidator(page))
                val validPrevs = sorted.take(i).filterNot(forwardInvalidator(page))
                val invalidNexts = sorted.drop(i + 1).filter(backwardInvalidator(page))
                val validNexts = sorted.drop(i + 1).filterNot(backwardInvalidator(page))
                if (invalidPrevs.isEmpty() && invalidNexts.isEmpty()) continue
                sorted = validPrevs + invalidNexts + listOf(page) + invalidPrevs + validNexts
                break
            }
            val valid = validator(sorted)
            if (valid) break
        }
        sorted
    }
    val sumOfFixedMiddlePages = fixedUpdates.map { it[it.size / 2] }.map(String::toInt).sumOf { it }
    println(sumOfFixedMiddlePages)
}

typealias Predicate<T> = (T) -> Boolean
typealias PageInvalidator = (String) -> Predicate<String>
fun getConstraintValidator(constraints: Map<String, Set<String>>): PageInvalidator = {
    page -> { constraints.getOrDefault(page, setOf()).contains(it) }
}

fun getValidator(forwardInvalidator: PageInvalidator, backwardInvalidator: PageInvalidator): Predicate<List<String>> = {
    it.mapIndexed { i, page ->
        it.take(i).none(forwardInvalidator(page)) && it.drop(i + 1).none(backwardInvalidator(page))
    }.all { x -> x }
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
