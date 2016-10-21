#!/usr/bin/env Rscript
args = commandArgs(trailingOnly=TRUE)
rank <- scan(args[1])
pdf(file=args[2])
barplot(rank, main = "Rank N", xlab = "Rank")
dev.off()
