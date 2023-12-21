using ParameterInfo.RebalancingOracleDescriptions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HedgingEngine.RebalancingOracle
{
    internal class FixedTimesOracle : IRebalacingOracle
    {
        public int Period { get; set; }
        public int compteur = 0;
        public FixedTimesOracle(IRebalancingOracleDescription rebalancingOracleDescription) {
            FixedTimesOracleDescription fixedOracle = (FixedTimesOracleDescription)rebalancingOracleDescription;
            this.Period = fixedOracle.Period;
        
        }
        bool IRebalacingOracle.rebalancingDate(DateTime date)
        {
            compteur++;

            if ((compteur) % Period == 0)
            {
                return true;
            }
            return false;
        }
    }
}
