#!/usr/bin/env Rscript
pdf(file="roc.pdf")

par(mgp = c(2.2,0.45,0), tcl = -0.4, mar = c(3.3,3.3,3.3,1.1))
plot(c(0:1.0), c(0:1.0), xlab = "FAR", ylab = "1 - FRR", type="n", xlim = range(0:1), ylim = range(0:1.0), xaxs="i", yaxs="i")
grid(col = "lightgray", lty = "dotted", lwd = par("lwd"), equilogs = TRUE)
par(new = TRUE)
plot(c(0:1.0), c(0:1.0), xlab = "FAR", ylab = "1 - FRR", type="n", xlim = range(0:1), ylim = range(0:1.0), xaxs="i", yaxs="i")
legend("bottomright", inset=.05, cex = 1, c("Impressão digital - detecção manual", "Impressão digital - detecção automática", "Iris - detecção manual", "Iris + Impressão digital - manual", "Aleatório"), lty=c(1,1,1,1,5), lwd=c(2,2,2,2,2), col=c("blue", "red", "darkgreen", "green", "black"), bg="grey96")

far <- scan("fingerprint/true/far.txt")
frr <- scan("fingerprint/true/frr.txt")
par(new = TRUE)
plot(far, 1 - frr, col = "blue", pch = 16, type = "l", xaxs = "i", yaxs = "i", axes = FALSE, ann = FALSE, xlim = range(0:1), ylim = range(0:1.0))

far <- scan("fingerprint/automatic/far.txt")
frr <- scan("fingerprint/automatic/frr.txt")
par(new = TRUE)
plot(far, 1 - frr, col = "red", pch = 16, type = "l", xaxs = "i", yaxs = "i", axes = FALSE, ann = FALSE, xlim = range(0:1), ylim = range(0:1.0))

far <- scan("iris/solo/far.txt")
frr <- scan("iris/solo/frr.txt")
par(new = TRUE)
plot(far, 1 - frr, col = "darkgreen", pch = 16, type = "l", xaxs = "i", yaxs = "i", axes = FALSE, ann = FALSE, xlim = range(0:1), ylim = range(0:1.0))

far <- scan("iris/and_fingerprint/far.txt")
frr <- scan("iris/and_fingerprint/frr.txt")
par(new = TRUE)
plot(far, 1 - frr, col = "green", pch = 16, type = "l", xaxs = "i", yaxs = "i", axes = FALSE, ann = FALSE, xlim = range(0:1), ylim = range(0:1.0))

par(new = TRUE)
plot(c(0:1.0), c(0:1.0), lty = 5, col = "black", pch = 16, type = "l", xaxs = "i", yaxs = "i", axes = FALSE, ann = FALSE, xlim = range(0:1), ylim = range(0:1.0))

dev.off()
