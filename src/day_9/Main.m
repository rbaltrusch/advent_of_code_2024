classdef Main
    methods(Static)
        function blocks = method()
            % Part 1: rearrange files into blocks of empty memory by
            % fragmenting them.
            blocksizes = fileread("data.txt");
            blocks = Main.determine_memory_layout(blocksizes);
            fragmented_blocks = Main.rearrange_blocks(blocks);
            fprintf("Memory Checksum: %i\n", Main.calculate_checksum(fragmented_blocks));

            % Part 2: rearrange files into blocks of empty memory without fragmentation.
            unfragmented_blocks = Main.rearrange_whole_files(blocks);
            fprintf("Memory Checksum: %i\n", Main.calculate_checksum(unfragmented_blocks));
        end

        function blocks = determine_memory_layout(blocksizes)
            split_string = split(blocksizes, "");
            array_size = sum(cellfun(@str2num, split_string(2:end-1)));

            blocks = zeros(array_size, 1);
            id = 1; % IDs are 1-offset until checksum calculation, 0 means free memory
            offset = 0;
            for c = 1:numel(blocksizes)
                blocksize = str2double(blocksizes(c));
                if (mod(c, 2) == 1)
                    start = c + offset;
                    stop = start + blocksize - 1;
                    blocks(start:stop) = id;
                    id = id + 1;
                end
                offset = offset + blocksize - 1;
            end
        end

        function blocks = rearrange_blocks(blocks)
            index = 1;
            vacant_block_indices = find(blocks == 0);
            occupied_block_indices = find(blocks ~= 0);
            for i = numel(occupied_block_indices):-1:1
                block_index = occupied_block_indices(i);
                next_vacant_block_index = vacant_block_indices(index);
                if (next_vacant_block_index >= block_index)
                    break
                end
                value = blocks(block_index);
                blocks(next_vacant_block_index) = value;
                blocks(block_index) = 0;
                index = index + 1;
            end
        end

        function blocks = rearrange_whole_files(blocks)
            vacant_block_indices = find(blocks == 0);
            occupied_block_indices = find(blocks ~= 0);
            file_index = numel(occupied_block_indices);
            while(file_index > 0)
                block_index = occupied_block_indices(file_index);
                value = blocks(block_index);
                window_size = min(10, block_index);
                file_blocks = blocks(block_index - window_size + find(blocks(block_index - window_size + 1 : block_index) == value));
                file_size = numel(file_blocks);
                file_index = file_index - file_size;

                for i = 1:numel(vacant_block_indices)
                    next_vacant_block_index = vacant_block_indices(i);
                    if (next_vacant_block_index > block_index)
                        break
                    end

                    next_vacant_block_end_index = next_vacant_block_index + file_size - 1;
                    if (all(blocks(next_vacant_block_index : next_vacant_block_end_index) == 0))
                        blocks(next_vacant_block_index : next_vacant_block_index + file_size - 1) = value;
                        blocks(block_index - file_size + 1 : block_index) = 0;
                        break
                    end
                end
            end
        end

        function checksum = calculate_checksum(blocks)
            nonzero_block_indices = find(blocks ~= 0);
            nonzero_blocks = blocks(blocks ~= 0);
            % Note: the 1 offset converts 1-based ID indices to 0-based indices.
            checksum = sum((nonzero_block_indices - 1) .* (nonzero_blocks - 1));
        end
    end
end
