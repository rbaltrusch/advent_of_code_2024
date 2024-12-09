{-# LANGUAGE OverloadedStrings #-}

import qualified Data.Text as T
import qualified Data.Text.IO as IO

-- calculates all possible results applying the mapper to the specified numbers
type Mapper = Integer -> Integer -> [Integer]
calculate :: Mapper -> [Integer] -> [Integer] -> [Integer]
calculate _ result [] = result
calculate mapper result (x:xs) = calculate mapper updated xs where
    updated = if null result then [x] else concatMap (mapper x) result

type Algorithm = [Integer] -> [Integer]
calculatePart1 :: Algorithm
calculatePart1 = calculate mapper [] where mapper x y = [x + y, x * y]

calculatePart2 :: Algorithm
calculatePart2 = calculate mapper [] where
    mapper x y = [x + y, x * y, join x y]
    join x y = read (show y <> show x) :: Integer

matches :: Algorithm -> (Integer, [Integer]) -> Bool
matches algorithm x = elem target . algorithm $ numbers where
    target = fst x
    numbers = snd x

readInput :: T.Text -> [(Integer, [Integer])]
readInput = map (tuple . split) . T.lines where
    split = map toInt . T.splitOn " " . T.replace ":" ""
    toInt x = read . T.unpack $ x :: Integer
    tuple (x:xs) = (x, xs)

main :: IO ()
main = do
    text <- IO.readFile "data.txt"
    let input = readInput text
    print . sum . map fst . filter (matches calculatePart1) $ input
    print . sum . map fst . filter (matches calculatePart2) $ input
