#!/usr/bin/env Rscript
pdf(file="nthrank.pdf")

par(mgp = c(2.2,0.45,0), tcl = -0.4, mar = c(3.3,3.3,3.3,1.1))
plot(c(1:50), c(1:50), xlab = "N", ylab = "Rank-N", type="n", xlim = range(1:50), ylim = range(1:50), xaxs="i", yaxs="i")
grid(col = "lightgray", lty = "dotted", lwd = par("lwd"), equilogs = TRUE)
legend("bottomright", inset=.02, cex = 0.75, c("Impressão digital - detecção manual", "Impressão digital - detecção automática", "Iris - detecção manual", "Iris + Impressão digital - manual"), lty=c(1,1,1,1), lwd=c(2,2,2,2), col=c("blue", "red", "darkgreen", "green"), bg="grey96")

par(new = TRUE)
rank <- scan("fingerprint/true/nthrank.txt")
plot(c(1:50), rank, col="blue", pch=16, type="l", axes = FALSE, ann = FALSE, xaxs="i", yaxs="i", xlim = range(1:50), ylim = range(1:50))

par(new = TRUE)
rank <- scan("fingerprint/automatic/nthrank.txt")
plot(c(1:50), rank, type='l', pch=16, col="red", axes = FALSE, ann = FALSE, xaxs="i", yaxs="i", xlim = range(1:50), ylim = range(1:50))

par(new = TRUE)
rank <- scan("iris/solo/nthrank.txt")
plot(c(1:50), rank, type='l', pch=16, col="darkgreen", axes = FALSE, ann = FALSE, xaxs="i", yaxs="i", xlim = range(1:50), ylim = range(1:50))

dev.off()
