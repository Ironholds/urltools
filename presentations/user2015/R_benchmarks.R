library(microbenchmark)
library(urltools)
library(ggplot2)
library(ggthemes)
encoded_urls <- rep("Hinrichtung_auf_dem_Altst%c3%a4dter_Ring.JPG%2f120px-Hinrichtung_auf_dem_Altst%c3%a4dter_Ring.JPG",
                    10000)

#Python figure is 41.176300048828125

base_result <- data.frame(platform = "Base R",
                          timings = microbenchmark({test <- lapply(encoded_urls, URLdecode)})$time,
                          stringsAsFactors = FALSE)

urltools_result <- data.frame(platform = "urltools",
                              timings = microbenchmark({test <- url_decode(encoded_urls)})$time,
                              stringsAsFactors = FALSE)
results <- rbind(base_result, urltools_result)
results$timings <- results$timings*1.0e-6

ggsave(filename = "./presentations/user2015/timings.png",
       plot = ggplot(results, aes(platform, timings)) + 
         geom_violin(fill="#0072B2") + 
         coord_flip() + scale_y_log10() + theme_fivethirtyeight() +
         labs(title = "Decoding 10,000 URLs, 100 times, with base R and with urltools",
              x = "Method", y = "Time taken (ms)") + 
         theme(axis.title = element_text(size = 16), axis.text = element_text(size = 14))
)