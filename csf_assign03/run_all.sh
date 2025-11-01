
sizes=("256" "512")
assoc=("1" "4" "8")
block_sizes=("16")
write_policies=("write-allocate" "no-write-allocate")
back_policies=("write-back" "write-through")
replacement=("lru" "fifo")
for s in "${sizes[@]}"; do
  for a in "${assoc[@]}"; do
    for b in "${block_sizes[@]}"; do
      for wp in "${write_policies[@]}"; do
        for wb in "${back_policies[@]}"; do
          for r in "${replacement[@]}"; do
            echo "Running: ./csim $s $a $b $wp $wb $r < gcc.trace"
            ./csim $s $a $b $wp $wb $r < gcc.trace > "out_${s}_${a}_${b}_${wp}_${wb}_${r}.txt"
          done
        done
      done
    done
  done
done