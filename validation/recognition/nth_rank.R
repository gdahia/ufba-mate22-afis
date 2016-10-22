#!/usr/bin/env Rscript
args = commandArgs(trailingOnly=TRUE)
rank <- scan(args[1])
pdf(file=args[2])
par(mgp=c(2.2,0.45,0), tcl=-0.4, mar=c(3.3,3.3,3.3,1.1))
barplot(table(rank), main = "Rank-N", xlab = "", col=args[3])
dev.off()
