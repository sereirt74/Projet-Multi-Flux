using MarketData;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HedgingEngine.Portfolio
{
    public class Portfolio
    {
        public double Cash { get; set; }
        public double PortfolioValue { get; set; }
        public double[] Deltas { get; set; }
        
        
        public Portfolio(double portfolioValue, int nbAssets)
        {
            PortfolioValue = portfolioValue;
            Deltas = new double[nbAssets];
        }

        public void updateComposition(double[] newDeltas, DataFeed dataFeed)
        {
            if (newDeltas.Length != Deltas.Length)
            {
                Console.WriteLine("Erreur : Les tableaux n'ont pas la même longueur.");
            }

            double somme = 0;
            Deltas = newDeltas;
            for (int i = 0; i < Deltas.Length; ++i)
            {
                somme += Deltas[i] * dataFeed.SpotList.Values.ToArray()[i];
            }
            Cash = PortfolioValue - somme;
        }

        public void UpdatePortfolioValue(DataFeed dataFeed, double rebalancingTime, double riskFreeRate)
        {
            Cash = Math.Exp(riskFreeRate * rebalancingTime);
            double somme = 0;
            for (int i = 0; i < Deltas.Length; i++)
            {
                somme += Deltas[i] * dataFeed.SpotList.Values.ToArray()[i];
                
            }
            PortfolioValue = somme + Cash;
        }
    }


}